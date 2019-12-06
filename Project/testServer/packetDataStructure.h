#pragma once
#pragma pack(push,1)
struct packetHead { 	// ��� �ְ�޴� ��Ŷ���� ��� 
	char id;			// Ŭ���̾�Ʈ ID
	char packetType;         // ��Ŷ�� Ÿ��
};

struct simplePacket {	// ������ ó���� ��Ŷ��
	char id;			// Ŭ���̾�Ʈ ID
	float value;		// �̺�Ʈ���� �� ( �ӵ�, ����, ��Ʈ � )
	char packetType;	// ��Ŷ�� ����	
};

struct shootPacket {	// �߻� �̺�Ʈ�� ��Ŷ��
	char id;			// Ŭ���̾�Ʈ ID
	short mposX;
	short mPosY;
	short tarPosX;
	short tarPosY;
};

struct posPacket {		// Ŭ���̾�Ʈ���� ��ǥ�� �����Ҷ�
	char id;			// Ŭ���̾�Ʈ ID
	short posX;			// Ŭ���̾�Ʈ xy
	short posY;
};

struct allPacket{		// ���ʿ� �ѹ��� ������ ��� ������ Ŭ���̾�Ʈ�鿡�� ���� (����ȭ��)
	char id;			// Ŭ���̾�Ʈ�� ID
	float x;
	float y;			// Ŭ���̾�Ʈ�� x,y ��ǥ
	float velx;
	float vely;			// Ŭ���̾�Ʈ�� ���ӵ� x y
};

struct itemPacket {
	short effect;
	short itemPosX;
	short itemPosY;
};
#pragma pack(pop)

enum packetType : char {
	E_PACKET_SPEED = 0,	// �������� �ӵ� ����
	E_PACKET_DEGREE,	// �������� �̵� ���� ����
	E_PACKET_SHOOT, // �������� �߻� ��û
	E_PACKET_HIT,	// �������� �ǰ�
	E_PACKET_DIE,	// �������� ���
	E_PACKET_SENID,	// ó�� �����鿡�� ID �� ������� ��Ŷ
	E_PACKET_OTSET,	// �ٸ� �������� �����ϱ����� ��Ŷ
	E_PACKET_SYNC,	// ����ȭ�� ���� ��Ŷ
	E_PACKET_ITEM,
	E_PACKET_WIND,
	E_PACKET_DELT,	// � ������Ʈ�� ����
	E_PACKET_GETITEM,
	E_PACKET_REEF,
	E_PACKET_COLLREEF
};
