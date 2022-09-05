////////////////////////////////////////////////////////////////
// color.hlsl
// HLSL : High Level Shader Language
//                    -> 과거에는 Assembler
//                    -> C 와 유사했다.
// * 사용되는 function 이름은 동일 이름으로 Shader 를 생성해준다.
//
////////////////////////////////////////////////////////////////

// 정점에 대해 보낼 데이터를 맞춰주는 역할이다.
struct VertexInput
{
    float4 position : POSITION0; // Sementics Name [n] 은 시멘틱스 넘버
    float4 color    : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION0; // System Value
    float4 color    : COLOR0;
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
    
    matrix world;   // 나자신에 대한 데이터
    matrix view;    // 눈 (카메라)이 보는 방향
    matrix proj;    // Projecter 화면을 비출때 2D 는 직교사영
    
};

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
    output.color    = input.color;
    
    return output;
}

//////////////////////////////////////////////////////
// PixelShader 프로그램
//
//////////////////////////////////////////////////////

float4 PS(PixelInput input) : SV_Target // 현재 세팅된 메인렌더타겟 을 사용하라고 알려준다.
{
    return input.color;   
}
