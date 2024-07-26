CONTIKI_PROJECT = Energy-based IDS
APPS+=powertrace collect-view
PROJECT_SOURCEFILES += collect-sample.c math-operations.c ids-energy-based.c
all: $(CONTIKI_PROJECT)

CONTIKI = ../..
CONTIKI_WITH_RIME = 1
TARGET_LIBFILES += -lm
V=1
#CUSTOM_RULE_C_TO_CO=1
#CUSTOM_RULE_C_TO_O=1
#CUSTOM_RULE_LINK=1
#CUSTOM_RULE_C_TO_OBJECTDIR_O=1
include $(CONTIKI)/Makefile.include

ifdef CUSTOM_RULE_C_TO_CO
%.co: %.c
	$(TRACE_CC)
	$(Q)$(CC) $(CFLAGS) -DAUTOSTART_ENABLE -c $< -o $@ -lm
endif

ifdef CUSTOM_RULE_C_TO_O
%.o: %.c
	$(TRACE_CC)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@ -lm
endif

ifdef CUSTOM_RULE_C_TO_OBJECTDIR_O
$(OBJECTDIR)/%.o: %.c | $(OBJECTDIR)
	$(TRACE_CC)
	$(Q)$(CC) $(CFLAGS) -MMD -c $< -o $@ -lm
	@$(FINALIZE_DEPENDENCY)
endif

ifdef CUSTOM_RULE_LINK
%.$(TARGET): %.co $(PROJECT_OBJECTFILES) $(PROJECT_LIBRARIES) contiki-$(TARGET).a
	$(TRACE_LD)
	$(Q)$(LD) $(LDFLAGS) $(TARGET_STARTFILES) ${filter-out %.a,$^} \
	    ${filter %.a,$^} $(TARGET_LIBFILES) -o $@ -lm

endif
