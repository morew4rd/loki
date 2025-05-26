default: help

demo:
	./odin run examples/demo/demo.odin -file

report:
	./odin report

help:
	@echo "make <target> <optional_params>"
	@echo "---"
	@cat Makefile

debug:
	./build_odin.sh debug

release:
	./build_odin.sh release

release-native:
	./build_odin.sh release-native

release_native:
	./build_odin.sh release-native

nightly:
	./build_odin.sh nightly

ci-send-devbuild:
	git tag devbuild
	git push --tags
	git push --delete origin devbuild
	git tag --delete devbuild

ci-send-devtest:
	git tag devtest
	git push --tags
	git push --delete origin devtest
	git tag --delete devtest