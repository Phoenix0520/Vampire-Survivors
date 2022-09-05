///////////////////////////////////////////////////////////////////////
// color.hlsl
// HLSL : High Level Shader Language
//                    -> 과거에는 Assembler
//                    -> C 와 유사했다.
//  *사용되는 function 이름은 동일 이름으로 Shader 를 생성해준다.
//
//  PNG -> 투명도가 있는 그림파일
//  BMP -> 투명도가 없는 그림파일
//
//  이미지는 무조건 UV 좌표가 있는데. 텍스쳐를 입힐때 UV 좌표를 사용한다.
//
//
///////////////////////////////////////////////////////////////////////

// 정점에 대해 보낼 데이터를 맞춰주는 역할이다.
struct VertexInput
{
    float4 position : POSITION0; // Sementics Name [n] 은 시멘틱스 넘버
    float2 uv : TEXCOORD0; // Texture cooredinate
};

struct PixelInput
{
    float4 position : SV_POSITION0; // System Value
    float2 uv : TEXCOORD0;
};

// 상수 버퍼를 받을 역할을 하는 셰이더이다.

cbuffer Transformbuffer : register(b0) // 0~13 슬롯에 등록 (4096 * 16byte 이하 16배수)
{
    // matrix 는 (float) 4x4 의 형태로 이루어져있다.
    // 그림의 크기를 변경하기 위해서는 scale
    // 위치를 이동하기 위해서는 trans
    // 회전하기 위해서는 rotate
    // 이와같은 것을 편하게 사용하기 위해 matrix 를 사용한다.
    // 정규화된 좌표만 가지고 있는 DX 에서 실제 화면에서 필요한 값을 구하기위해 matrix 를 곱해주는것이다.
    
    matrix world; // 나자신에 대한 데이터
    matrix view; // 눈 (카메라)이 보는 방향
    matrix proj; // Projecter 화면을 비출때 2D 는 직교사영
    
};

cbuffer ColorBuffer : register(b1) // 4byte 의 배수로 보내줘야한다
{
    float4 textColor; // 바뀔 칼라값
    int path; // switch 문 사용시의 Tag
    float time;
    float dummy1;
    float dummy2;
}

Texture2D sourc_texture1 : register(t0); // Texture 자원 이라는 뜻
Texture2D sourc_texture2 : register(t1); // 또다른 Texture 를 입힐때 사용한다
// Texture 는 하나의 셰이더에 128개 까지 넣을수 있다.

SamplerState samp : register(s0); // 확대 축소 (Sampling) 를 할때 사용한다.
// SampleState 는 0~15 까지 넣을 수 있다.

//////////////////////////////////////////////////////
// VertexShader 프로그램
//
//////////////////////////////////////////////////////

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, world); // 월드 변환
    output.position = mul(output.position, view); // 뷰 변환
    output.position = mul(output.position, proj); // 프로젝션 변환
    output.uv = input.uv;
    
    return output;
}

//////////////////////////////////////////////////////
// PixelShader 프로그램
//
//////////////////////////////////////////////////////

float4 PS(PixelInput input) : SV_Target // 현재 세팅된 메인렌더타겟 을 사용하라고 알려준다.
{
    float4 color = sourc_texture1.Sample(samp, input.uv); // Texture 를 통과한 내용이 color 에 들어가게된다.
    float4 returnColor = color;
    float4 alpha = float4(color.rgb, 0); // 투명
    float4 alpha2 = color; // 표준색
    
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
            returnColor = lerp(textColor, alpha, time); // 선형 보간, 기존의 alpha ~ 0 
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