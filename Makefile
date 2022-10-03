ifeq ($(Building_Yocto), 1)
KERNEL_SRC := $(KDIR)
unexport Building_Yocto
else
# specified buildroot kernel src
KERNEL_SRC :=
endif



HAS_PM_DOMAIN ?= 1
DEBUG ?= 0

ifeq ($(DEBUG),1)
EXTRA_CFLAGS1 += -DCONFIG_ADLAK_DEBUG=1
endif


ifeq ($(HAS_PM_DOMAIN),0)
EXTRA_CFLAGS1 += -DCONFIG_HAS_PM_DOMAIN=0
endif


EXTRA_INCLUDE := -I $(PWD)/adla/kmd/drv/port/platform/linux
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/port/platform
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/port/os/linux/mm
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/port/os/linux
EXTRA_INCLUDE += -I $(PWD)adla/kmd/drv/port/os
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/port
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/uapi/linux
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/uapi
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/common/mm
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv/common
EXTRA_INCLUDE += -I $(PWD)/adla/kmd/drv

file_adlak_version := $(PWD)/adla/kmd/drv/common/adlak_version.h



modules:generate-files
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD)/adla/kmd  modules "EXTRA_LDFLAGS+=$(EXTRA_LDFLAGS1)" "EXTRA_CFLAGS+= -Wno-error $(EXTRA_CFLAGS1)  $(EXTRA_INCLUDE)"

include $(PWD)/VERSION
ADLAK_V_MAJOR := $(strip $(ADLA_DRIVER_V_MAJOR))
ADLAK_V_MINOR := $(strip $(ADLA_DRIVER_V_MINOR))
ADLAK_V_PATCH := $(strip $(ADLA_DRIVER_V_PATCH))

FORCE:

# Generate a temporary adlak_version.h
generate-files: $(file_adlak_version)
$(file_adlak_version):FORCE
	@echo "Generate $@"
	@echo "" > $@
	@echo /\* This file is auto generated\*/ >> $@
	@echo \#ifndef __ADLAK_VERSION_H__ >> $@
	@echo \#define __ADLAK_VERSION_H__ >> $@
	@echo \#define ADLAK_VERSION \"$(ADLAK_V_MAJOR).$(ADLAK_V_MINOR).$(ADLAK_V_PATCH)\" >> $@
	@echo "" >> $@
	@echo \#define ADLAK_VERSION_MAJOR $(ADLAK_V_MAJOR) >> $@
	@echo \#define ADLAK_VERSION_MINOR $(ADLAK_V_MINOR) >> $@
	@echo \#define ADLAK_VERSION_RSV $(ADLAK_V_PATCH) >> $@
	@echo '#define ADLAK_VERSION_VAL  ((ADLAK_VERSION_MAJOR << 16) + ((ADLAK_VERSION_MINOR) << 8) + (ADLAK_VERSION_RSV))' >> $@
	@echo '#endif /* __ADLAK_VERSION_H__ end define*/' >> $@
	@echo "" >> $@
del-generate-files: $(file_adlak_version)
	@echo  "Remove the temporary files...\n $^"
	@rm -fv $^


all:modules


clean:del-generate-files
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD)/adla/kmd clean

help:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD)/adla/kmd help

