#!/usr/bin/env bash
set -euo pipefail

msvc_lib="${MSVC_ROOT:-$HOME/msvc}/bin/x64/lib.exe"
rsp="$(mktemp --suffix=.rsp "${TMPDIR:-/tmp}/msvc-lib-rsp.XXXXXX")"

cleanup() {
    rm -f "$rsp"
}
trap cleanup EXIT HUP INT TERM

printf '%s\r\n' "$@" > "$rsp"

"$msvc_lib" "@$(winepath -w "$rsp")"
