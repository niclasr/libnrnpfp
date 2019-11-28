MKC_CHECK_FUNCLIBS= sendfile:sendfile

MKC_CHECK_HEADERS= sys/param.h sys/sendfile.h

MKC_CHECK_PROTOTYPES= sendfile_org sendfile_fbsd

MKC_PROTOTYPE_FUNC.sendfile_org    = ssize_t sendfile(int,int,off_t*,size_t)
MKC_PROTOTYPE_HEADERS.sendfile_org = sys/sendfile.h

MKC_PROTOTYPE_FUNC.sendfile_fbsd   = int sendfile(int,int,off_t,size_t,struct sf_hdtr *,off_t *,int)
MKC_PROTOTYPE_HEADERS.sendfile_fbsd= sys/types.h sys/socket.h sys/uio.h

.include <mkc.configure.mk>
