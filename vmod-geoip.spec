Summary: GeoIP VMOD for Varnish
Name: vmod-geoip
Version: 1.0
Release: 1%{?dist}
License: BSD
Group: System Environment/Daemons
Source0: libvmod-geoip.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Requires: varnish > 4.0
BuildRequires: make, python-docutils


%description
A varnish vmod for geoip detection based on the client ip


%prep
%setup -n libvmod-geoip


%build
./configure --prefix=/usr/ --docdir='${datarootdir}/doc/%{name}'
make
make check


%install
make install DESTDIR=%{buildroot}


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%{_libdir}/varnish/vmods/
%doc /usr/share/doc/%{name}/*
%{_mandir}/man?/*


%changelog
* Mon May 04 2015 Michael Bischof <mb@byteworks.ch> - 1.1-1
- Updated to be compatible with varnish 4

* Fri May 09 2014 Michael Bischof <mb@byteworks.ch> - 1.0-1
- Initial version.

