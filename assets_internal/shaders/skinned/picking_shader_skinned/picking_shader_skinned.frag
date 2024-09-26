#version 460 core

out float Fragvalue;

in VS_OUT
{
    flat uint meshDrawId;
} fs_in;

void main()
{
    Fragvalue = float(fs_in.meshDrawId);
}
