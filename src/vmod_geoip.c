/**
 * vmod_geoip.c
 *
 * Copyright (c) 2014 by Byteworks GmbH
 * All rights reserved
 *
 * http://www.byteworks.ch/
 *
 * This vmod has been inspired by:
 * - https://github.com/varnish/libvmod-example
 *
 */

#include <stdlib.h>
#include <GeoIP.h>

#include "config.h"

#include "vrt.h"
#include "vrt_obj.h"
#include "bin/varnishd/cache.h"

#include "vsm.h"
#include "vcc_if.h"


typedef struct {
	char *file;
	char *default_name;
	char *default_code;
	GEOIP_API GeoIP *handle;
} geoip_t;


static void
free_priv(struct vmod_priv *priv)
{
	geoip_t *geoip = (geoip_t *) priv->priv;

	if (geoip->file) free(geoip->file);
	if (geoip->default_name) free(geoip->default_name);
	if (geoip->default_code) free(geoip->default_code);
	if (geoip->handle) GeoIP_delete(geoip->handle);

	free(priv->priv);
}


static void
init_priv(struct vmod_priv *priv)
{
	priv->priv = malloc(sizeof(geoip_t));
	
	// Register cleanup handler
	// The "free" function will be called with the "priv" pointer as only argument.
	priv->free = free_priv;
}


int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
	init_priv(priv);
}


static void
init_geoip(geoip_t *geoip)
{
	if (geoip->file) {
		geoip->handle = GeoIP_open(geoip->file, GEOIP_MMAP_CACHE);
	}
	else {
		geoip->handle = GeoIP_new(GEOIP_MMAP_CACHE);
	}

	GeoIP_set_charset(geoip->handle, GEOIP_CHARSET_UTF8);
}


void
vmod_config(struct sess *sp, struct vmod_priv *priv, const char *name, const char *value)
{
	geoip_t *geoip = (geoip_t *) priv->priv;

	CHECK_OBJ_NOTNULL(sp, SESS_MAGIC);
	CHECK_OBJ_NOTNULL(sp->ws, WS_MAGIC);
	assert(priv);
	assert(name);
	assert(value);

	if (strcmp(name, "file") == 0) {
		geoip->file = strdup(value);
	}

	if (strcmp(name, "default_code") == 0) {
		geoip->default_code = strdup(value);
	}

	if (strcmp(name, "default_name") == 0) {
		geoip->default_name = strdup(value);
	}
}


const char *
vmod_country_name(struct sess *sp, struct vmod_priv *priv, const char *ip)
{
	const char *temp = NULL;
	geoip_t *geoip = priv->priv;

	CHECK_OBJ_NOTNULL(sp, SESS_MAGIC);
	CHECK_OBJ_NOTNULL(sp->ws, WS_MAGIC);
	assert(priv);
	assert(ip);

	if (!geoip->handle) init_geoip(geoip);

	temp = GeoIP_country_name_by_addr(geoip->handle, ip);

	return WS_Dup(sp->wrk->ws, 
		temp ? temp :
		geoip->default_name ? geoip->default_name :
		""
	);
}


const char *
vmod_country_code(struct sess *sp, struct vmod_priv *priv, const char *ip)
{
	const char *temp = NULL;
	geoip_t *geoip = priv->priv;

	CHECK_OBJ_NOTNULL(sp, SESS_MAGIC);
	CHECK_OBJ_NOTNULL(sp->ws, WS_MAGIC);
	assert(priv);
	assert(ip);

	if (!geoip->handle) init_geoip(geoip);

	temp = GeoIP_country_code_by_addr(geoip->handle, ip);

	return WS_Dup(sp->wrk->ws, 
		temp ? temp :
		geoip->default_code ? geoip->default_code :
		""
	);
}


const char *
vmod_version(struct sess *sp)
{
	return VERSION;
}

