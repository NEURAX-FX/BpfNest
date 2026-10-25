#!/usr/bin/env bash
# Fetch the pinned upstream ZygiskNext API4 header for local builds.
# The header has no redistribution grant, so it is intentionally not in this repo.
set -euo pipefail
ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
DEST="$ROOT/third_party/zygisk_next_api.h"
COMMIT=2620542f29284672c9e869e285fceafe68ee6259
SHA256=957922aeec1bb31091cc37dca29267b7fa41e4e109f679af1526ee4c5e4325c8
URL="https://raw.githubusercontent.com/LSPosed/ZygiskNext/$COMMIT/zygisk_next_api.h"

[[ $# == 0 ]] || { printf 'Usage: %s\n' "$0" >&2; exit 2; }
command -v curl >/dev/null || { printf 'ERROR: curl is required\n' >&2; exit 1; }
command -v sha256sum >/dev/null || { printf 'ERROR: sha256sum is required\n' >&2; exit 1; }

if [[ -f "$DEST" ]]; then
    actual=$(sha256sum "$DEST" | awk '{print $1}')
    if [[ "$actual" == "$SHA256" ]]; then
        printf 'API4 header already present and pinned: %s\n' "$DEST"
        exit 0
    fi
    printf 'ERROR: %s exists but its SHA256 is %s (expected %s).\n' "$DEST" "$actual" "$SHA256" >&2
    printf 'Move the unexpected file away before fetching the pinned header.\n' >&2
    exit 1
fi

mkdir -p "$ROOT/third_party"
tmp=$(mktemp "${DEST}.tmp.XXXXXX")
trap 'rm -f "$tmp"' EXIT
printf 'Fetching %s\n' "$URL"
curl -fsSL --retry 3 --proto '=https' "$URL" -o "$tmp"
printf '%s  %s\n' "$SHA256" "$tmp" | sha256sum -c -
mv "$tmp" "$DEST"
trap - EXIT
printf 'Fetched pinned API4 header: %s\n' "$DEST"
printf 'Upstream reserves all rights; this repository does not redistribute the header.\n'
