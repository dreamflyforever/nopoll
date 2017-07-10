/*
 *  LibNoPoll: A websocket library
 *  Copyright (C) 2015 Advanced Software Production Line, S.L.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  For commercial support on build Websocket enabled solutions
 *  contact us:
 *          
 *      Postal address:
 *         Advanced Software Production Line, S.L.
 *         Edificio Alius A, Oficina 102,
 *         C/ Antonio Suarez Nº 10,
 *         Alcalá de Henares 28802 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/nopoll
 */
#include <nopoll.h>
#include <config.h>

nopoll_bool debug = nopoll_false;
nopoll_bool show_critical_only = nopoll_false;

noPollCtx * create_ctx (void) {
	
	/* create a context */
	noPollCtx * ctx = nopoll_ctx_new ();

	return ctx;
}

nopoll_bool test_01 (void) {
	noPollCtx  * ctx;
	noPollConn * conn;

	/* create context */
	ctx = create_ctx ();

	/* check connections registered */
	if (nopoll_ctx_conns (ctx) != 0) {
		printf ("ERROR: expected to find 0 registered connections but found: %d\n", nopoll_ctx_conns (ctx));
		return nopoll_false;
	} /* end if */

	nopoll_ctx_unref (ctx);

	/* reinit again */
	ctx = create_ctx ();

	/* call to create a connection */
	conn = nopoll_conn_new (ctx, "localhost", "8080", NULL, NULL, NULL, NULL);
	if (! nopoll_conn_is_ok (conn)) {
	        printf ("ERROR: Expected to find proper client connection status, but found error (conn=%p, conn->session=%d, NOPOLL_INVALID_SOCKET=%d)..\n",
			conn, (int) nopoll_conn_socket (conn), (int) NOPOLL_INVALID_SOCKET);
		return nopoll_false;
	}

	/* check connections registered */
	if (nopoll_ctx_conns (ctx) != 1) {
		printf ("ERROR: expected to find 1 registered connections but found: %d\n", nopoll_ctx_conns (ctx));
		return nopoll_false;
	} /* end if */

	/* ensure connection status is ok */
	if (! nopoll_conn_is_ok (conn)) {
		printf ("ERROR (3): expected to find proper connection status, but found failure.. (conn=%p, conn->session=%d, NOPOLL_INVALID_SOCKET=%d)..\n",
			conn, (int) nopoll_conn_socket (conn), (int) NOPOLL_INVALID_SOCKET);
		return nopoll_false;
	}

	printf ("Test 01: reference counting for the connection: %d\n", nopoll_conn_ref_count (conn));

	/* check if the connection already finished its connection
	   handshake */
	while (! nopoll_conn_is_ready (conn)) {

		if (! nopoll_conn_is_ok (conn)) {
			printf ("ERROR (4.1 jkd412): expected to find proper connection handshake finished, but found connection is broken: session=%d, errno=%d : %s..\n",
				(int) nopoll_conn_socket (conn), errno, strerror (errno));
			return nopoll_false;
		} /* end if */

		/* wait a bit 10ms */
		nopoll_sleep (10000);
	} /* end if */
	/* send content text(utf-8) */
	if (nopoll_conn_send_text (conn, "This is a test", 14) != 14) {
		printf ("ERROR: Expected to find proper send operation..\n");
		return nopoll_false;
	} else {
		printf ("SUCCESS..\n");
	}

	/* finish connection */
	nopoll_conn_close (conn);
	
	/* finish */
	nopoll_ctx_unref (ctx);

	return nopoll_true;
}


int main (int argc, char ** argv)
{
	if (test_01 ()) {	
		printf ("Test 01: Simple connect and disconnect [   OK   ]\n");
	} else {
		printf ("Test 01: Simple connect and disconnect [ FAILED ]\n");
		return -1;
	}
	/* call to cleanup */
	nopoll_cleanup_library ();
	printf ("All tests ok!!\n");

	return 0;
}

/* end-of-file-found */
