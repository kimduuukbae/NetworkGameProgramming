#pragma once
#pragma pack(push,1)
struct packetHead { 	// 모든 주고받는 패킷들의 헤더 
	char id;			// 클라이언트 ID
	char packetType;         // 패킷의 타입
};

struct simplePacket {	// 간단한 처리의 패킷용
	char id;			// 클라이언트 ID
	float value;		// 이벤트들의 값 ( 속도, 각도, 히트 등… )
	char packetType;	// 패킷의 종류	
};

struct shootPacket {	// 발사 이벤트의 패킷용
	char id;			// 클라이언트 ID
	short mposX;
	short mPosY;
	short tarPosX;
	short tarPosY;
};

struct posPacket {		// 클라이언트들의 좌표를 정의할때
	char id;			// 클라이언트 ID
	short posX;			// 클라이언트 xy
	short posY;
};

struct allPacket{		// 몇초에 한번씩 서버의 모든 정보를 클라이언트들에게 보냄 (동기화용)
	char id;			// 클라이언트의 ID
	float x;
	float y;			// 클라이언트의 x,y 좌표
};
#pragma pack(pop)

enum packetType : char {
	E_PACKET_SPEED = 0,	// 누군가의 속도 변경
	E_PACKET_DEGREE,	// 누군가의 이동 각도 변경
	E_PACKET_SHOOT, // 누군가의 발사 요청
	E_PACKET_HIT,	// 누군가의 피격
	E_PACKET_DIE,	// 누군가의 사망
	E_PACKET_SENID,	// 처음 유저들에게 ID 를 보내기용 패킷
	E_PACKET_OTSET,	// 다른 유저들을 세팅하기위한 패킷
	E_PACKET_SYNC	// 동기화를 위한 패킷
};
