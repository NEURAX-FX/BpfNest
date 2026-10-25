# Sourced by the supported manager's real installer, never executed directly.
# Default extraction is intentional: no fabricated updater or service script.
if [ "$BOOTMODE" != true ]; then
  abort "! Recovery installation is not supported; use Magisk/KernelSU manager"
fi
if [ -n "$KSU" ]; then
  [ "${KSU_VER_CODE:-0}" -ge 11575 ] || abort "! KernelSU Manager/ksud 11575 or newer required"
  command -v magisk >/dev/null 2>&1 && abort "! Multiple root implementations are not supported"
elif [ -n "$MAGISK_VER_CODE" ]; then
  [ "$MAGISK_VER_CODE" -ge 26402 ] || abort "! Magisk 26402 or newer required"
else
  abort "! Only Magisk or KernelSU manager installation is supported"
fi
[ "$ARCH" = arm64 ] || abort "! Only arm64 is supported"
[ "$API" -ge 26 ] || abort "! Android API26 or newer required"
[ -f "$MODPATH/libbpfnest.so" ] || abort "! Missing native library"
[ -f "$MODPATH/zn_modules.txt" ] || abort "! Missing ZygiskNext scope"
[ -f "$MODPATH/bpfnest.conf" ] || abort "! Missing observe configuration"
set_perm_recursive "$MODPATH" 0 0 0755 0644
set_perm "$MODPATH/libbpfnest.so" 0 0 0644
set_perm "$MODPATH/customize.sh" 0 0 0644
ui_print "- BpfNest: shipped mode=observe, no attach parameters changed"
ui_print "- Requires ZygiskNext with API4 support; target version is loader-negotiated"
ui_print "! Patch is experimental and NOT netd-restart-safe; do not enable in production"
ui_print "- Nothing is injected into the running netd by this installer"
ui_print "- Activation/disable/uninstall takes effect on your next controlled reboot"
