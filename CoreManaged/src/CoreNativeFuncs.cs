using System.Runtime.InteropServices;

namespace Xnor.Core
{
    internal partial class CoreNativeFuncs
    {
        protected class StringHelper {

            public delegate string StringDelegate(string message);
            static StringDelegate stringDelegate = new StringDelegate(CreateString);

            [DllImport("CoreNative", EntryPoint="RegisterStringCallback_Core")]
            public static extern void SWIGRegisterStringCallback_Core(StringDelegate stringDelegate);

            static string CreateString(string cString)
            {
                return cString;
            }

            static StringHelper()
            {
                SWIGRegisterStringCallback_Core(stringDelegate);
            }
        }

        private static StringHelper stringHelper = new StringHelper();

        [DllImport("CoreNative")]
        public static extern string NativeComponentListGetItem(nint componentList, uint index, out nint item);
    }
}
