
PATH.sfplib := ${.PARSEDIR:tA}

DPINCDIRS += ${PATH.sfplib}
LDFLAGS += ${PATH.sfplib}/libsendfileportable.a
