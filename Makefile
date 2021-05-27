.PHONY: clean All

All:
	@echo "----------Building project:[ 4MbToMidnight - Release ]----------"
	@"$(MAKE)" -f  "4MbToMidnight.mk"
clean:
	@echo "----------Cleaning project:[ 4MbToMidnight - Release ]----------"
	@"$(MAKE)" -f  "4MbToMidnight.mk" clean
