//48byte
struct Material
{
	//16
	//���� - 16byte�� ��� �϶� 
    float3 diffuse;
    float shiness;
	
	//�̷��� �� ���� 16����Ʈ�� ���� �־�� �Ѵ� Vector3�� ���޾� ���� ���� Vector3 ������ x ��Ʈ�� ���� ����� �ȴ�.

	//16
    float3 ambient;
    float dummy1;

	//16
    float3 specular;
    float dummy2;

	//4 <-�������� �����Ͱ� ���� �ʾƼ� ������ �����.
    float dummy3;
};