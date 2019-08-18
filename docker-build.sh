#!/usr/bin/env bash
docker build -t amiiswap .
docker run --rm -v ${PWD}:/output:rw amiiswap cp /build/AmiiSwap.{elf,nro,nso,pfs0,nacp} /output