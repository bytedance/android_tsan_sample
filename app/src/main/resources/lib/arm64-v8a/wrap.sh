#!/system/bin/sh

HERE="$(cd "$(dirname "$0")" && pwd)"

echo "CMD: $@"

export TSAN_OPTIONS="ignore_noninstrumented_modules=1,handle_segv=0,handle_sigbus=0,handle_sigfpe=0"

$HERE/tsan_loader "$@"
