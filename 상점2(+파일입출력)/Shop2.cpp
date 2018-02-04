#include "Shop2.h"



Shop2::Shop2()
{
}

Shop2::~Shop2()
{
}

#define MAX_SIZE 1000

bool Shop2::ItemSetup()
{
	// item.txt �о�ͼ� �ֱ�
	FILE* infile;
	
	// ������ �ִ��� Ȯ��
	if ((infile = fopen("item.txt", "r")) == 0) {
		printf("item.txt ���� �б� ����\n");
		return false;
	}

	// ���Ϸκ��� �о�� ���ڿ� ���� ����
	char str[MAX_SIZE];

	// ������ ������ ���� ����ü
	tagItemInfo item;

	// ������ ������ �б�
	while (feof(infile) == 0) {
		fgets(str, MAX_SIZE, infile);

		// ���ҵ� ���ڿ� ��ġ�� �����ϱ� ���� ����
		int cnt = 0;

		// strtok�� [ �� ] �� ��� ����
		char* token = strtok(str, "[]");

		// ��� ���� �ɶ����� �ݺ�
		while (token != NULL) {
			switch (cnt) {
			case 0:	// �������� ����
				item.itemKind = (ITEM)atoi(token);
				break;
			case 1: // �������� �̸�
				item.name = token;
				break;
			case 2: // �������� ����
				item.price = atoi(token);
				break;
			case 3: // �������� Ư��
				item.attribute = atoi(token);
				break;
			case 4: // �������� ����
				item.description = token;
				break;
			case 5: // �������� ����
				item.count = atoi(token);
				cnt = 0;	// �ٽ� 0���� �ʱ�ȭ
				m_vItem.push_back(item); // �������� ���Ϳ� �ֱ�
				break;
			}
			// ���� ���ڿ� ��������
			token = strtok(NULL, "[]");
			cnt++;
		}
	}

	fclose(infile);	// ���� �ݱ�
	printf("item.txt ���� �б� ����\n");
	return true;
}

int Shop2::ItemOutput(int num)
{
	int itemCount = 0;
	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// ���� ������ ������ ������ ���
		if (m_viItem->itemKind != num) continue;

		cout << "==================================" << endl;
		cout << "������ ��ȣ : " << ++itemCount << endl;
		cout << "�̸� : " << m_viItem->name << endl;
		cout << "���� : " << m_viItem->price << endl;
		cout << "�ɷ�ġ : " << m_viItem->attribute << endl;
		cout << "���� : " << m_viItem->description << endl;
		cout << "���� : ";
		// ���� ������ (����) ? �� : ����
		(m_viItem->count > 0)
			? cout << m_viItem->count << endl
			: cout << "����" << endl;
		cout << "==================================" << endl;
	}
	return itemCount;
}

tagItemInfo Shop2::BuyItem(int itemKind, int num, int & gold)
{
	tagItemInfo buyItem;
	buyItem.itemKind = ITEM_EMPTY;
	int itemCount = 0;
	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// ���� ������ �����۸� ���
		if (m_viItem->itemKind != itemKind) continue;

		// ���� ������ ��ȣ�� ���� ��
		if (++itemCount == num) {
			// ������ ���� ���� ��
			if (gold >= m_viItem->price) {
				// ������ ������ ������ �������� ��
				if (m_viItem->count > 0) {
					cout << m_viItem->name << "�� "
						<< m_viItem->price << "gold�� ����" << endl;
					gold -= m_viItem->price;
					m_viItem->count--;
					buyItem = *m_viItem;	// ������ ���� �־���
				}
				else {
					cout << "��� ����" << endl;
				}
				// ������ ã�����Ƿ� ��
				break;
			}
		}
	}
	return buyItem;
}

void Shop2::AddItem(tagItemInfo item)
{
	bool isNewItem = true;

	m_viItem = m_vItem.begin();
	for (m_viItem; m_viItem != m_vItem.end(); ++m_viItem) {
		// ������ ���� �������� Ȯ��
		if (m_viItem->itemKind != item.itemKind) continue;

		// ������ �̸� ������ ������ ����
		if (m_viItem->name.compare(item.name) == 0) {
			m_viItem->count++;
			isNewItem = false;
			break;
		}
	}

	if (isNewItem == true) {
		m_vItem.push_back(item);
	}
}
