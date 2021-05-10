float base;
float bias;


struct PS_INPUT
{
	float4 vColor : COLOR0;
};


float4 main(in PS_INPUT pin) : COLOR0
{
	return pin.vColor * bias / base;
}