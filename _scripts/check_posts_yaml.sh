#!/bin/bash - 
#===============================================================================
#
#          FILE: check_posts_yaml.sh
# 
#         USAGE: ./check_posts_yaml.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: YOUR NAME (), 
#  ORGANIZATION: 
#       CREATED: 06/29/2017 17:06
#      REVISION:  ---
#===============================================================================

rm -f post_yaml_check

for i in $(head -15 2014-12-12-episodio-2.md | sed /---/d | cut -d: -f1 | gsed -E 's/\s+//g')
   do grep --color=never -E "^\s*${i}" * >> post_yaml_check | column -t -s: 
done


