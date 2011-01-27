#include <u.h>
#include <libc.h>
#include <fcall.h>
#include <thread.h>
#include <9p.h>

#include "bf.h"


void do_read_tape(Req* r)
{
	char*	data = bf_get_data();

	readstr( r, data );	

	free( data );

	return;
}

void do_cmd_tape(Req* r)
{
	// data size	:= r->ifcall.count;
	// data		:= r->ifcall.data 
	
	//int i = 0;

	bf_do_cmd( r->ifcall.count, r->ifcall.data );

	return;
}

enum{
	Qcmd,
	Qtape,
};

void fsopen(Req *r)
{
	respond( r, nil );

	return;
}

void fsread(Req *r)
{
	int qid;

	qid = (int)r->fid->qid.path;

	switch( qid ){
		case Qcmd:
			// do nothing
			break;
		case Qtape:
			do_read_tape( r );
			break;
		default:
			break;
	}

	respond( r, nil );

	return;
}

void fswrite(Req *r)
{
	int qid;

	qid = (int)r->fid->qid.path;

	switch( qid ){
		case Qcmd:
			do_cmd_tape( r );
			break;
		case Qtape:
			// do nothing
			break;
		default:
			break;
	}

	respond( r, nil );

	return;
}

void fsdestroyfile(File *file)
{
	return;
}

Srv bffs = {
	.open	= fsopen,
	.read	= fsread,
	.write	= fswrite,
};

void initfs(void)
{
	bffs.tree = alloctree( nil, nil, DMDIR|077, fsdestroyfile );
	createfile( bffs.tree->root, "cmd", nil, 0777, nil );
	createfile( bffs.tree->root, "data", nil, 0777, nil );

	return;
}

void main(int argc, char** argv)
{
	initfs();

	postmountsrv( &bffs, "bf", nil, MREPL|MCREATE );
}
