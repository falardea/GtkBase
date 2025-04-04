#!/bin/bash

THIS_FILENAME=$(basename $0)
ARG=$1

echo "running $THIS_FILENAME $ARG"

sassc -t expanded ./resources/style/scss/this_app_name_style.scss ./resources/style/this_app_name_style.css
