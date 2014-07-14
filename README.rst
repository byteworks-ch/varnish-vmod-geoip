==========
vmod_geoip
==========

--------------------
Varnish GeoIP Module
--------------------

:Author: Michael Bischof
:Date: 2014-05-09
:Version: 1.0
:Manual section: 3

SYNOPSIS
========

import geoip;

DESCRIPTION
===========

Functions to handle country detection based on the client ip address.


FUNCTIONS
=========

config
------

Prototype
        ::

                config(STRING NAME, STRING VALUE)
Return value
	NONE
Description
	Configures the geoip module. Supported options are:
        
        file: Specify the database file
        default_name: The default country name returned if not found
        default_code: The default country code returned if not found
Example
        ::

                geoip.config("file", "/tmp/geoip.db");

country_name
------------

Prototype
        ::

                country_name(STRING IP)
Return value
	STRING
Description
	Returns the detected country name by ip.
Example
        ::

                set resp.http.x-country-name = geoip.country_name(client.ip);

country_code
------------

Prototype
        ::

                country_code(STRING IP)
Return value
	STRING
Description
	Returns the detected country code by ip.
Example
        ::

                set resp.http.x-country-code = geoip.country_code(client.ip);


INSTALLATION
============

The source tree is based on autotools to configure the building, and
does also have the necessary bits in place to do functional unit tests
using the varnishtest tool.

Usage::

 ./configure VARNISHSRC=DIR [VMODDIR=DIR]

`VARNISHSRC` is the directory of the Varnish source tree for which to
compile your vmod. Both the `VARNISHSRC` and `VARNISHSRC/include`
will be added to the include search paths for your module.

Optionally you can also set the vmod install directory by adding
`VMODDIR=DIR` (defaults to the pkg-config discovered directory from your
Varnish installation).

Make targets:

* make - builds the vmod
* make install - installs your vmod in `VMODDIR`
* make check - runs the unit tests in ``src/tests/*.vtc``

In your VCL you could then use this vmod along the following lines::
	
	import geoip;

	sub vcl_deliver {
		# This sets resp.http.x-country-name to the detected country name
		set resp.http.x-country-name = geoip.country_name(client.ip);
	}

HISTORY
=======

This manual page was released as part of the varnish-vmod-geoip package.
For further information about vmods in general check the vmod directory:
https://www.varnish-cache.org/vmods

COPYRIGHT
=========

This document is licensed under the same license as the
varnish-vmod-geoip project. See LICENSE for details.

* Copyright (c) 2014 by Byteworks GmbH

