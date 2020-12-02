SUBDIRS += NSReader
SUBDIRS += NSLogger
SUBDIRS += NSMonitor

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)
	@set -e; for dir in $(SUBDIRS); do $(MAKE) -C $${dir} $@; done

clean:
	@set -e; for dir in $(SUBDIRS); do $(MAKE) -C $${dir} $@; done
	@rm -f omninames-*
	@rm -f rtc.conf .confFilePath
