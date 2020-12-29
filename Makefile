#!/usr/bin/make -f
# Makefile for DPF #
# ---------------- #
# Created by falkTX
#

include Makefile.base.mk

all: dgl examples gen

# --------------------------------------------------------------

dgl:
	$(MAKE) -C dgl
	$(MAKE) all -C plugins/triceratops



ifneq ($(CROSS_COMPILING),true)
gen: examples utils/lv2_ttl_generator
	@$(CURDIR)/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/utils/generate-vst-bundles.sh
endif

utils/lv2_ttl_generator:
	$(MAKE) -C utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------
install:
	mkdir ~/.config/triceratops -p
	cp $(CURDIR)/delirium_ui/logo.png ~/.config/triceratops 
	chmod 777 ~/.config/triceratops -R
	cp $(CURDIR)/bin/thunderox_triceratops.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/bin/thunderox_triceratops-dssi /usr/lib/dssi -r
	cp $(CURDIR)/bin/thunderox_triceratops-dssi.so /usr/lib/dssi
	cp $(CURDIR)/bin/thunderox_triceratops-vst.so /usr/lib/vst
	chmod +x $(CURDIR)/bin/thunderox_triceratops
	cp $(CURDIR)/bin/thunderox_triceratops /usr/bin


# --------------------------------------------------------------
uninstall:
	rm ~/.config/triceratops -R
	rm /usr/lib/lv2/thunderox_triceratops.lv2 -rf
	rm /usr/lib/dssi/thunderox_triceratops.dssi -rf
	rm /usr/lib/dssi/thunderox_triceratops-dssi.so -f
	rm /usr/lib/vst/thunderox_triceratops0.so -f
	rm /usr/bin/thunderox_triceratops

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dgl
	$(MAKE) clean -C plugins/triceratops
	$(MAKE) clean -C utils/lv2-ttl-generator

	rm -rf bin build
	rm -f delirium_ui/*.d
	rm -f delirium_ui/*.o

# --------------------------------------------------------------

.PHONY: dgl examples
