include ../../Makefile.include
DEPS = ../../Makefile.include Makefile ../../download-files.include

# lib name, version
LIBNAME=libass
VERSION=0.17.1-branch
BASE_URL=https://github.com/libass/libass/archive/refs/heads
SOURCE=$(VERSION)
ARCHIVE=$(SOURCE).tar.gz
SHA512=f7fdaed039a89d396e1cfcc0e4ce07901e5d39428169f1ea1da71e63597b2e72046acd94179aca678af97d12d816fb5bf627342e3792e12bff17870d05abefa3
include ../../download-files.include

ifeq ($(OS),android)
  # Android API Level 21/22 requires explicit link.
  # This doesnt appear to be required for API Level 23+ (Android 6+)
  export LDFLAGS+= -lstdc++
  
  ifeq ($(CPU),i686)
    export ASFLAGS+= -DPIC=1
  endif
endif

# configuration settings
CONFIGURE=cp -f $(CONFIG_SUB) $(CONFIG_GUESS) .; \
          ./configure --prefix=$(PREFIX) --disable-enca

LIBDYLIB=$(PLATFORM)/$(LIBNAME)/.libs/$(LIBNAME).a

all: .installed-$(PLATFORM)


$(PLATFORM): $(DEPS) | $(TARBALLS_LOCATION)/$(ARCHIVE).$(HASH_TYPE)
	rm -rf $(PLATFORM)/*; mkdir -p $(PLATFORM)
	cd $(PLATFORM); $(ARCHIVE_TOOL) $(ARCHIVE_TOOL_FLAGS) $(TARBALLS_LOCATION)/$(ARCHIVE)
	cd $(PLATFORM); $(AUTORECONF) -vif
	cd $(PLATFORM); $(CONFIGURE)

$(LIBDYLIB): $(PLATFORM)
	$(MAKE) -C $(PLATFORM)

.installed-$(PLATFORM): $(LIBDYLIB)
	$(MAKE) -C $(PLATFORM) install
	touch $@

clean:
	$(MAKE) -C $(PLATFORM) clean
	rm -f .installed-$(PLATFORM)

distclean::
	rm -rf $(PLATFORM) .installed-$(PLATFORM)

