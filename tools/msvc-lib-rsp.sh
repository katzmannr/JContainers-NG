#!/usr/bin/env bash
# wine-lib-rsp.sh
set -euo pipefail

MSVC_ROOT="${MSVC_ROOT:-$HOME/msvc}"

# Source the same environment that lib.exe's wrapper uses
. "$MSVC_ROOT/bin/x64/msvcenv.sh"

# Use the same lib.exe path as your toolchain wrapper
msvc_lib="$MSVC_ROOT/bin/x64/lib.exe"

rsp="$(mktemp --suffix=.rsp "${TMPDIR:-/tmp}/wine-lib-rsp.XXXXXX")"
trap 'rm -f "$rsp"' EXIT HUP INT TERM

# Convert each argument:
# - Options like /nologo, /machine:x64, /out:... stay as-is.
# - File paths are converted to absolute Unix, then to Windows paths.
{
    for arg in "$@"; do
        case "$arg" in
            /nologo|/machine:*|/out:*|/LTCG|/IGNORE:*|/SUBSYSTEM:*)
                # MSVC options: leave unchanged
                printf '%s\r\n' "$arg"
                ;;
            *)
                # Assume it's a file path (object, lib, etc.)
                # Make it absolute
                case "$arg" in
                    /*)
                        abs="$arg"
                        ;;
                    *)
                        abs="$PWD/$arg"
                        ;;
                esac
                # Convert to Windows path
                win="$(winepath -w "$abs")"
                printf '%s\r\n' "$win"
                ;;
        esac
    done
} > "$rsp"

# Now call lib.exe with an RSP that already contains Windows paths.
# Do NOT rely on wine-msvc.sh to rewrite these.
exec "$msvc_lib" "@$(winepath -w "$rsp")"

exit "$rc"
