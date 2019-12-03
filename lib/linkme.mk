
PATH.libnrnpfp := ${.PARSEDIR:tA}

DPINCDIRS += ${PATH.libnrnpfp}
LDFLAGS += ${PATH.libnrnpfp}/libnrnpfp.a
