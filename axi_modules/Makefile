.PHONY: format
format:
	echo "[ format ]"
	find include/ src/ test/ -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' | \
		xargs /usr/bin/clang-format -i -style=file

.PHONY: format-check
format-check:
	echo "[ format ]"
	find include/ src/ test/ -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' | \
		xargs /usr/bin/clang-format -i -style=file -output-replacements-xml | \
		grep -c "<replacement " | \
		grep 0 > /dev/null || ( echo "ERROR: Run 'make clang-format' to fix code formatting"; exit 1 )
