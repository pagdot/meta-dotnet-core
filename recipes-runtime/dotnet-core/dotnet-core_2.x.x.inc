###################################################################################################
# Contains additional parameters for the recipe to download the release binaries from Microsoft
# pertaining to all 2.x.x versions.
# Copyright Richard Dunkley 2018
###################################################################################################
DESCRIPTION = "This does not build this version, it only pulls the binaries from Microsoft"
BUGTRACKER = "https://github.com/dotnet/core/issues"
SECTION = "runtime"

S = "${WORKDIR}/dotnet-${PV}"

require recipes-runtime/dotnet-core/dotnet-core_mit_2.x.x.inc

DEPENDS = "zlib"
RDEPENDS_${PN} = "icu lttng-tools lttng-ust zlib curl libgssglue"

INSANE_SKIP_${PN} += "already-stripped libdir textrel"
INSANE_SKIP_${PN}-dbg += "libdir"
INSANE_SKIP_${PN} += "staticdev"

do_configure[noexec] = "1"
do_compile[noexec] = "1"

#PACKAGES =+ "${PN}-dbg ${PN}-staticdev ${PN}"

#FILES_${PN} += "${datadir}"
#FILES_${PN} += ""

FILES_${PN} += " \
	${datadir}/dotnet/dotnet \
	${datadir}/dotnet/*.txt \
	${datadir}/dotnet/host/fxr/${PV}/*.so \
	${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/.version \
	${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/*.so \
	${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/*.dll \
	${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/*.json \
	${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/*.txt \
	/usr/local/share/dotnet \
	"

do_install:prepend () {
	install -d ${D}${bindir}
	install -d ${D}${datadir}/dotnet/
	install -m 0755 ${S}/dotnet ${D}${datadir}/dotnet/
	install -m 0644 ${S}/LICENSE.txt ${D}${datadir}/dotnet/
	install -m 0644 ${S}/ThirdPartyNotices.txt ${D}${datadir}/dotnet/

	install -d ${D}${datadir}/dotnet/host/fxr/${PV}/
	install -m 0755 ${S}/host/fxr/${PV}/libhostfxr.so ${D}${datadir}/dotnet/host/fxr/${PV}/

	install -d ${D}${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/
	
	install -m 0644 ${S}/shared/Microsoft.NETCore.App/${PV}/.version ${D}${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/.version
	for file in ${S}/shared/Microsoft.NETCore.App/${PV}/*.so; do
		install -m 0755 "$file" ${D}${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/
	done
	for file in ${S}/shared/Microsoft.NETCore.App/${PV}/{*.dll,*.json,*.txt,*.a}; do
		install -m 0644 "$file" ${D}${datadir}/dotnet/shared/Microsoft.NETCore.App/${PV}/
	done

	install -d ${D}/usr/local/share

}

do_install:append() {
	cd ${D}/usr/local/share
	ln -s ../../share/dotnet ${D}/usr/local/share

	cd ${D}${bindir}
	ln -s ../share/dotnet/dotnet ${D}${bindir}/dotnet
}

