///////////////////////////////////////////////////////////////////////
// color.hlsl
// HLSL : High Level Shader Language
//                    -> ���ſ��� Assembler
//                    -> C �� �����ߴ�.
//  *���Ǵ� function �̸��� ���� �̸����� Shader �� �������ش�.
//
//  PNG -> ������ �ִ� �׸�����
//  BMP -> ������ ���� �׸�����
//
//  �̹����� ������ UV ��ǥ�� �ִµ�. �ؽ��ĸ� ������ UV ��ǥ�� ����Ѵ�.
//
//
///////////////////////////////////////////////////////////////////////

// ������ ���� ���� �����͸� �����ִ� �����̴�.
struct VertexInput
{
    float4 position : POSITION0; // Sementics Name [n] �� �ø�ƽ�� �ѹ�
    float2 uv       : TEXCOORD0; // Texture cooredinate
};

struct PixelInput
{
    float4 position : SV_POSITION0; // System Value
    float2 uv       : TEXCOORD0;
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

cbuffer ColorBuffer : register(b1)  // 4byte �� ����� ��������Ѵ�
{
    float4 Color;   // �ٲ� Į��
    float time1;
    float time2;
    float time3;
    float time4;
}

Texture2D sourc_texture1 : register(t0); // Texture �ڿ� �̶�� ��
Texture2D sourc_texture2 : register(t1); // �Ǵٸ� Texture �� ������ ����Ѵ�
// Texture �� �ϳ��� ���̴��� 128�� ���� ������ �ִ�.

SamplerState samp : register(s0); // Ȯ�� ��� (Sampling) �� �Ҷ� ����Ѵ�.
// SampleState �� 0~15 ���� ���� �� �ִ�.

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
    output.uv       = input.uv;
    
    return output;
}

//////////////////////////////////////////////////////
// PixelShader ���α׷�
//
//////////////////////////////////////////////////////

float4 PS(PixelInput input) : SV_Target // ���� ���õ� ���η���Ÿ�� �� ����϶�� �˷��ش�.
{
    
    float4 color = sourc_texture1.Sample(samp, input.uv); // Texture �� ����� ������ color �� ���Եȴ�.
    //float4 test = float4(Color.r, Color.g, Color.b, color.a);
    
    return color; 
}
