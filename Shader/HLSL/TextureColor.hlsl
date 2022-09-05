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
    float2 uv : TEXCOORD0; // Texture cooredinate
};

struct PixelInput
{
    float4 position : SV_POSITION0; // System Value
    float2 uv : TEXCOORD0;
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
    
    matrix world; // ���ڽſ� ���� ������
    matrix view; // �� (ī�޶�)�� ���� ����
    matrix proj; // Projecter ȭ���� ���⶧ 2D �� �����翵
    
};

cbuffer ColorBuffer : register(b1) // 4byte �� ����� ��������Ѵ�
{
    float4 textColor; // �ٲ� Į��
    int path; // switch �� ������ Tag
    float time;
    float dummy1;
    float dummy2;
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
    output.uv = input.uv;
    
    return output;
}

//////////////////////////////////////////////////////
// PixelShader ���α׷�
//
//////////////////////////////////////////////////////

float4 PS(PixelInput input) : SV_Target // ���� ���õ� ���η���Ÿ�� �� ����϶�� �˷��ش�.
{
    float4 color = sourc_texture1.Sample(samp, input.uv); // Texture �� ����� ������ color �� ���Եȴ�.
    float4 returnColor = color;
    float4 alpha = float4(color.rgb, 0); // ����
    float4 alpha2 = color; // ǥ�ػ�
    
    [forcecase]
    
    switch (path)
    {
        // Nothing
        case 0:
            break;
        // Just Color
        case 1:
            returnColor = float4(textColor.r, textColor.g, textColor.b, color.a);
            break;
        // lerp
        case 2:
            if (color.a <= 0.0f)
                discard;
            returnColor = lerp(textColor, alpha, time); // ���� ����, ������ alpha ~ 0 
            break;
        // cut top bottom animation
        case 3:
            if (input.position.y < dummy1)
                discard;
            if (input.position.y > dummy2)
                discard;
            break;
        // Change All
        case 4:
            if (color.a <= 0.15f)
                discard;
            returnColor = float4(textColor.r, textColor.g, textColor.b, textColor.a);
            break;
        // Color to White
        case 5:
            alpha = float4(textColor.rgb, alpha2.a);
        
            returnColor = lerp(alpha, alpha2, time);
            break;
        // dont think
        case 6:
            alpha = float4(color.rgba);
            alpha2 = float4(textColor.rgb, alpha2.a);
        
            returnColor = lerp(alpha, alpha2, time);
        
            break;
        // Just Alpha
        case 7:
            if (color.a <= 0.125f)
                discard;
            returnColor = float4(color.r, color.g, color.b, textColor.a);
            break;
        // Remove Magenta (bugs Appear)
        case 8:
            if (color.r == 1.0f && color.g == 0.0f && color.b == 1.0f)
                discard;
            returnColor = float4(color.rgba);
            break;
        // Blending Color & Input
        case 9:
            returnColor = float4(color.r * textColor.r, color.g * textColor.g, color.b * textColor.b, color.a);
            break;
        // Remove Almost Magenta
        case 10:
            if (color.r == 139.0f / 255.0f && color.g == 139.0f / 255.0f && color.b == 139.0f / 255.0f)
                returnColor.b = 0.0f;
            break;
        // Pulse (red , blue)
        case 11:
            {
            //if (time >= 0.75f)
            //    returnColor = float4(0.0f, 0.0f, time, color.a);
            //if (time >= 0.5f)
            //    returnColor = float4(0.75f - time, time, time, color.a);
            //else if (time >= 0.3f)
            //    returnColor = float4(1.0f, 1.0f - time, 1.0f - time, color.a);
            //else
            //    returnColor = float4(time, 0.0f, 0.75f - time, color.a);
            //    break;
            
            
                if (time >= 0.5f)
                    //returnColor = float4(time - 1.0f, 0.0f, time, 1.0f);
                    returnColor = float4(1.0 - time, 0.0f, time, 1.0f);
                else
                    returnColor = float4(time, 0.0f, 1.0 - time, 1.0f);
                    //returnColor = float4(time, 0.0f, time - 1.0f, 1.0f);
                
                //float4 color1 = float4(0.0f, 0.0f, 1.0f, 1.0f);
                //float4 color2 = float4(0.5f, 1.0f, 1.0f, 1.0f);
                //float4 color3 = float4(1.0f, 0.5f, 0.75f, 1.0f);
                //float delta = time * 3.0f;
                //
                //
                //if (time >= 0.66f)
                //    returnColor = lerp(color3, color1, delta);
                //else if (time >= 0.33f)   
                //    returnColor = lerp(color2, color3, delta);
                //else
                //    returnColor = lerp(color1, color2, delta);
            
                break;
            }
        // Remove Border & Change Color
        case 12:
            if (textColor.a <= 0.1f)
                returnColor = float4(textColor.r, textColor.g, textColor.b, color.a);
            break;
        // Remove Grey to Applied Percent
        case 13:
            if (color.r == 139.0f / 255.0f && color.g == 139.0f / 255.0f && color.b == 139.0f / 255.0f)
                returnColor.a = textColor.a;
            break;
    }
    
    if (returnColor.r == 1.0f && returnColor.g == 0.0f && returnColor.b == 1.0f)
        discard;
    
    return returnColor;
}