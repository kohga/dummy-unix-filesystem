#include "filsys.h" 
#include <string.h>


FilSys::FilSys() : s_isize (0), s_fsize (0), s_nfree (0), s_ninode(0), s_flock (0), 
	s_ilock (0), s_fmod(0), s_ronly (0)
{
	memset(s_free , 0, sizeof(s_free) / sizeof(s_free [0] )); 
	memset(s_inode , 0, sizeof(s_inode) / sizeof(s_inode [0] )); 
	memset(s_time , 0, sizeof(s_time) / sizeof(s_time [0] )); 
	memset(pad , 0, sizeof(pad) / sizeof(pad [0]));
}

FilSys::~FilSys () { 
}

std::unique_ptr<FilSys> FilSys::parse(std::shared_ptr<BlockDevice> bd) { 
	bd->setIndex( BlockDevice::BLOCK_SIZE);
	std::unique_ptr<FilSys> fptr(new FilSys());
	fptr->s_isize = bd->readShort(); 
	fptr->s_fsize = bd->readShort(); 
	fptr->s_nfree = bd->readShort(); 
	for (int i = 0; i < ARR_SIZE(fptr->s_free ); ++i ) {
		fptr->s_free [i] = bd->readShort();
	}
	fptr->s_ninode = bd->readShort();
	for (int i = 0; i < ARR_SIZE(fptr->s_inode ); ++i ) { 
		fptr->s_inode [i] = bd->readShort();
	}
	fptr->s_flock = bd->readChar(); 
	fptr->s_ilock = bd->readChar(); 
	fptr->s_fmod = bd->readChar( ); 
	fptr->s_ronly = bd->readChar(); 
	for (int i = 0; i < ARR_SIZE(fptr->s_time); ++i ) {
		fptr->s_time [i] = bd->readShort( );
	}

	for (int i = 0; i < ARR_SIZE(fptr->pad); ++i) { 
		fptr->pad[i] = bd->readShort();
	}

	return std::move( fptr );
}
