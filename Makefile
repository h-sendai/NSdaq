SUBDIRS += NSReader
SUBDIRS += NSLogger

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)
	@set -e; for dir in $(SUBDIRS); do $(MAKE) -C $${dir} $@; done

clean:
	@set -e; for dir in $(SUBDIRS); do $(MAKE) -C $${dir} $@; done
	@rm -f omninames-*.bak omninames-*.log
	@rm -f rtc.conf
