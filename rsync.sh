#!/usr/bin/env bash
set -euo pipefail

#pem file is $1, remote host is $2, volume name is $3
SSH_KEY=$1
REMOTE=$2
VOLUME_NAME=$3

TMP_DIR="$(mktemp -d)"

cleanup() {
  rm -rf "$TMP_DIR"
}
trap cleanup EXIT

# Incremental pull from AWS volume to temp dir
rsync -az \
  --rsync-path="sudo rsync" \
  -e "ssh -i ${SSH_KEY}" \
  "${REMOTE}:/var/lib/docker/volumes/${VOLUME_NAME}/_data/" \
  "${TMP_DIR}/"

# Incremental push into local Docker volume using rsync image
docker run --rm \
  -v "${VOLUME_NAME}:/data" \
  -v "${TMP_DIR}:/backup:ro" \
  instrumentisto/rsync \
  rsync -az /backup/ /data/

echo "Done: ${VOLUME_NAME} synced incrementally from ${REMOTE} into local Docker volume."
