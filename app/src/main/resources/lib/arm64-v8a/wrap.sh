#!/system/bin/sh

HERE="$(cd "$(dirname "$0")" && pwd)"

echo "CMD: $@"

export TSAN_OPTIONS="ignore_noninstrumented_modules=1"

$HERE/tsan_loader "$@"
