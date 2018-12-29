#!/bin/bash

echo "This is a simple shell..."

while read -p "> " command; do
(
	$command
)
if [ "$command" == "exit" ]; then $command; fi
done
