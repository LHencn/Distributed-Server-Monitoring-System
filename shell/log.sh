#!/bin/bash

log_out() {
    echo "[`date` $0 $1] $2"
}

log_err() {
    echo "[`date` $0 $1] $2" >&2
}

log_out "$LINENO" "Print log to stdout"
log_err "$LINENO" "Print log to stderr"


