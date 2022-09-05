////////////////////////////////////////////////////////////////
// color.hlsl
// HLSL : High Level Shader Language
//                    -> ���ſ��� Assembler
//                    -> C �� �����ߴ�.
// * ���Ǵ� function �̸��� ���� �̸����� Shader �� �������ش�.
//
////////////////////////////////////////////////////////////////

// ������ ���� ���� �����͸� �����ִ� �����̴�.
struct VertexInput
{
    float4 position : POSITION0; // Sementics Name [n] �� �ø�ƽ�� �ѹ�
    float4 color    : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION0; // System Value
    float4 color    : COLOR0;
};

// ��� ���۸� ���� ������ �ϴ� ���̴��̴�.

cbuffer Transformbuffer : register(b0) // 0~13 ���Կ� ��� (4096 * 16byte ���� 16���)
{
    // matrix �� (float) 4x4 �� ���·� �̷�����ִ�.
    // �׸��� ũ�⸦ �����ϱ� ���ؼ��� scale
    // ��ġ�� �̵��ϱ� ���ؼ��� trans
    // ȸ���ϱ� ���ؼ��� rotate
    // �̿Ͱ��� ���� ���ϰ� ����ϱ� ���� matrix �� ����Ѵ�.
    // ����ȭ�� ��ǥ�� ������ �ִ� DX ���� ���� ȭ�鿡�� �ʿ��� ���� ���ϱ����� matrix �� �����ִ°��̴�.
    
    matrix world;   // ���ڽſ� ���� ������
    matrix view;    // �� (ī�޶�)�� ���� ����
    matrix proj;    // Projecter ȭ���� ���⶧ 2D �� �����翵
    
};

//////////////////////////////////////////////////////
// VertexShader ���α׷�
//
//////////////////////////////////////////////////////

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, world); // ���� ��ȯ
    output.position = mul(output.position, view); // �� ��ȯ
    output.position = mul(output.position, proj); // �������� ��ȯ
    output.color    = input.color;
    
    return output;
}

//////////////////////////////////////////////////////
// PixelShader ���α׷�
//
//////////////////////////////////////////////////////

float4 PS(PixelInput input) : SV_Target // ���� ���õ� ���η���Ÿ�� �� ����϶�� �˷��ش�.
{
    return input.color;   
}
