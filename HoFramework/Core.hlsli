//48byte
struct Material
{
	//16
	//주의 - 16byte씩 페어 일때 
    float3 diffuse;
    float shiness;
	
	//이렇게 한 쌍이 16바이트로 묶여 있어야 한다 Vector3를 연달아 쓰면 다음 Vector3 변수의 x 파트에 덮어 씌우게 된다.

	//16
    float3 ambient;
    float dummy1;

	//16
    float3 specular;
    float dummy2;

	//4 <-마지막은 데이터가 들어가지 않아서 구색만 맞춘것.
    float dummy3;
};