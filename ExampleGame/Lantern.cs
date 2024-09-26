using System;
using System.Runtime.InteropServices;
using Xnor.Core;

namespace Game
{
    public class Lantern : ScriptComponent
    {
        public float RotationSpeed = 1f;
        private float time;

        protected override void Begin()
        {
        }

        protected override void Update()
        {
            time += Time.DeltaTime * RotationSpeed;

            Transform.RotationEulerAngleZ = MathF.Sin(time) / 2f;
        }
    }
}
