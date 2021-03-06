
/*
 * machinarium.
 *
 * cooperative multitasking engine.
*/

#include <machinarium.h>
#include <machinarium_private.h>

MACHINE_API machine_tls_t*
machine_tls_create(void)
{
	mm_errno_set(0);
	mm_tls_t *tls;
	tls = malloc(sizeof(*tls));
	if (tls == NULL) {
		mm_errno_set(ENOMEM);
		return NULL;
	}
	tls->verify    = MM_TLS_NONE;
	tls->server    = NULL;
	tls->protocols = NULL;
	tls->ca_path   = NULL;
	tls->ca_file   = NULL;
	tls->cert_file = NULL;
	tls->key_file  = NULL;
	return (machine_tls_t*)tls;
}

MACHINE_API void
machine_tls_free(machine_tls_t *obj)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	mm_errno_set(0);
	if (tls->protocols)
		free(tls->protocols);
	if (tls->server)
		free(tls->server);
	if (tls->ca_path)
		free(tls->ca_path);
	if (tls->ca_file)
		free(tls->ca_file);
	if (tls->cert_file)
		free(tls->cert_file);
	if (tls->key_file)
		free(tls->key_file);
	free(tls);
}

MACHINE_API int
machine_tls_set_verify(machine_tls_t *obj, char *mode)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	if (strcasecmp(mode, "none") == 0)
		tls->verify = MM_TLS_NONE;
	else
	if (strcasecmp(mode, "peer") == 0)
		tls->verify = MM_TLS_PEER;
	else
	if (strcasecmp(mode, "peer_strict") == 0)
		tls->verify = MM_TLS_PEER_STRICT;
	else
		return -1;
	return 0;
}

MACHINE_API int
machine_tls_set_server(machine_tls_t *obj, char *name)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	mm_errno_set(0);
	char *string = strdup(name);
	if (string == NULL) {
		mm_errno_set(ENOMEM);
		return -1;
	}
	if (tls->server)
		free(tls->server);
	tls->server = string;
	return 0;
}

MACHINE_API int
machine_tls_set_protocols(machine_tls_t *obj, char *protocols)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	mm_errno_set(0);
	char *string = strdup(protocols);
	if (string == NULL) {
		mm_errno_set(ENOMEM);
		return -1;
	}
	if (tls->protocols)
		free(tls->protocols);
	tls->protocols = string;
	return 0;
}

MACHINE_API int
machine_tls_set_ca_path(machine_tls_t *obj, char *path)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	mm_errno_set(0);
	char *string = strdup(path);
	if (string == NULL) {
		mm_errno_set(ENOMEM);
		return -1;
	}
	if (tls->ca_path)
		free(tls->ca_path);
	tls->ca_path = string;
	return 0;
}

MACHINE_API int
machine_tls_set_ca_file(machine_tls_t *obj, char *path)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	mm_errno_set(0);
	char *string = strdup(path);
	if (string == NULL) {
		mm_errno_set(ENOMEM);
		return -1;
	}
	if (tls->ca_file)
		free(tls->ca_file);
	tls->ca_file = string;
	return 0;
}

MACHINE_API int
machine_tls_set_cert_file(machine_tls_t *obj, char *path)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	mm_errno_set(0);
	char *string = strdup(path);
	if (string == NULL) {
		mm_errno_set(ENOMEM);
		return -1;
	}
	if (tls->cert_file)
		free(tls->cert_file);
	tls->cert_file = string;
	return 0;
}

MACHINE_API int
machine_tls_set_key_file(machine_tls_t *obj, char *path)
{
	mm_tls_t *tls = mm_cast(mm_tls_t*, obj);
	mm_errno_set(0);
	char *string = strdup(path);
	if (string == NULL) {
		mm_errno_set(ENOMEM);
		return -1;
	}
	if (tls->key_file)
		free(tls->key_file);
	tls->key_file = string;
	return 0;
}

MACHINE_API int
machine_set_tls(machine_io_t *obj, machine_tls_t *tls_obj)
{
	mm_io_t *io = mm_cast(mm_io_t*, obj);
	mm_tlsio_error_reset(&io->tls);
	if (io->tls_obj) {
		mm_errno_set(EINPROGRESS);
		return -1;
	}
	io->tls_obj = mm_cast(mm_tls_t*, tls_obj);
	if (io->accepted)
		return mm_tlsio_accept(&io->tls, io->tls_obj);
	if (io->connected)
		return mm_tlsio_connect(&io->tls, io->tls_obj);
	mm_errno_set(ENOTCONN);
	return -1;
}
