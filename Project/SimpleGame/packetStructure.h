#pragma once
#pragma pack(1)
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
	int pos;			// �߻��ϴ� Ŭ���̾�Ʈ�� ��ǥ
	int targetPos;		// �߻� ������ ��ǥ
};
#pragma pack()

enum packetType : char {
	E_PACKET_SPEED = 0,	// �������� �ӵ� ����
	E_PACKET_DEGREE,	// �������� �̵� ���� ����
	E_PACKET_SHOOT, // �������� �߻� ��û
	E_PACKET_HIT,	// �������� �ǰ�
	E_PACKET_DIE,	// �������� ���
	E_PACKET_SENID,	// ó�� �����鿡�� ID �� ������� ��Ŷ
	E_PACKET_OTSET // �ٸ� �������� �����ϱ����� ��Ŷ
};