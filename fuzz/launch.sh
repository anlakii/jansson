#!/usr/bin/sh

sudo mkdir /tmp/honggfuzz-workdir
sudo mount -t tmpfs -o size=2048M tmpfs /tmp/honggfuzz-workdir

honggfuzz -i corpus -W /tmp/honggfuzz-workdir -V -n20 -w json.dict -t10 -T -- ./fuzz
