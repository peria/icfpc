#!/bin/bash

BASE_DIR=$(cd $(dirname $0)/..; pwd)
API_HOST=https://robovinci.xyz/api
PROBLEM_HOST=https://cdn.robovinci.xyz

function access_api () {
  if [ "${ICFPC2022_TOKEN}" = "" ] ; then
    echo "Get API token fromm https://robovinci.xyz/dashboard and set it as SANME_TOKEN"
    echo "export ICFPC2022_TOKEN=api_token_is_here"
    exit
  fi

  curl --header "Authorization: Bearer ${ICFPC2022_TOKEN}" $@
}

function display_usage () {
  echo "Usage: ${0} <command> [other options]"
  echo "    command := submit | source | initial | target | best | record"
  exit
}

function get_problem_target () {
  if [ $# = 0 ] ; then
    echo "Usage: $0 target <problem_id> [png|json]"
    echo "  Downloads the target problem in PNG/JSON format.  If no format is specified, we choose JSON."
    exit
  fi
  ext=json
  if [ $# -gt 1 ] ; then
    ext=$2
  fi

  id=$1
  uri=${PROBLEM_HOST}/imageframes/${id}.${ext}
  wget ${uri}
}

function get_problem_init () {
  if [ $# = 0 ] ; then
    echo "Usage: $0 initial <problem_id>"
    echo "  Downloads the initial state in JSON format."
    exit
  fi
  id=$1
  uri=${PROBLEM_HOST}/imageframes/${id}.initial.json
  wget ${uri}
}

function get_source_canvas () {
  if [ $# = 0 ] ; then
    echo "Usage: $0 source <problem_id> [png|json]"
    echo "  Downloads the source canvas in PNG/JSON format.  If no format is specified, we choose JSON."
    exit
  fi
  ext=json
  if [ $# -gt 1 ] ; then
    ext=$2
  fi

  id=$1
  uri=${PROBLEM_HOST}/sourcepngs/${id}.source.${ext}
  wget ${uri}
}

function best_submits () {
  access_api ${API_HOST}/results/user
}

function get_submission_record () {
  if [ $# = 0 ] ; then
    echo "Usage: $0 record <submission_id>"
    echo "  Shows the submission status"
    exit
  fi
  id=$1
  uri=${API_HOST}/submissions/${id}
  access_api $uri
}

function submit () {
  if [ $# -lt 2 ] ; then
    echo "Usage: $0 submit <problem_id> <ISL file>"
    echo "  Submit the ISL file to the problem"
    exit
  fi
  id=$1
  isl=$2
  uri=${API_HOST}/problems/${id}
  access_api -F file=@${isl} ${uri}
}

if [ $# = 0 ] ; then
  display_usage
fi

command=$1
case $command in
  "submit")
    submit $2 $3 ;;
  "source")
    get_source_canvas $2 $3 ;;
  "target")
    get_problem_target $2 $3 ;;
  "initial")
    get_problem_init $2 ;;
  "best")
    best_submits ;;
  "record")
    get_submission_record $2 ;;
  *)
    display_usage
esac