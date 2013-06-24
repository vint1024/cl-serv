#ifndef _FSEND

enum FPacketType{FPVoid,FPSize,FPName,FPData};

struct FPacket{
	FPacketType _type;
	BYTE _size;
	union {
		DWORD _len;
		BYTE _binary[255];
	}_data;
	FPacket(DWORD len){
		_type=FPSize;
		_size=sizeof(DWORD);
		_data._len=len;
	}
	FPacket(const char *fname){
		_type=FPName;
		if (strlen(fname)>255) throw "filename too long";
		if (strlen(fname)==0) throw "filename too short";
		_size=strlen(fname);
		memcpy((void*)_data._binary,(void*)fname,_size);
	}
	FPacket()
	{
		_type=FPVoid;
	}
};

#endif