using System;
using System.IO;
using System.Runtime.InteropServices;
using UnityEditor;
using UnityEngine;

namespace TypeTreeTools
{
    static class NativeDump
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void DumpStructDebug();
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void ExportStringData(string modulePath);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void ExportClassesJson(string modulePath);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void ExportStructData(string modulePath, string unityVersion, uint runtimePlatform);
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        delegate void ExportStructDump(string modulePath);
        [MenuItem("Tools/Dump/Export String Data")]
        static void DoExportStringData()
        {
            Invoke<ExportStringData>(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName);
        }
        [MenuItem("Tools/Dump/Export Classes JSON")]
        static void DoExportClassesJson()
        {
            Invoke<ExportClassesJson>(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName);
        }
        [MenuItem("Tools/Dump/Export Struct Data")]
        static void DoExportStructData()
        {
            Invoke<ExportStructData>(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName,
                Application.unityVersion,
                (uint)Application.platform);
        }
        [MenuItem("Tools/Dump/Export Struct Dump")]
        static void DoExportStructDump()
        {
            Invoke<ExportStructDump>(System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName);
        }
        [MenuItem("Tools/Dump/Dump Struct Debug")]
        static void DoTestDump()
        {
            Invoke<DumpStructDebug>();
        }
        public static bool Is64BitProcess { get { return IntPtr.Size == 8; } }

        static IntPtr LoadTypeTreeLib()
        {
            var libraryPath = Is64BitProcess ?
                "CustomPlugins/x86_64/NativeTypeTreeTools.dll" :
                "CustomPlugins/x86/NativeTypeTreeTools.dll";
            libraryPath = Path.GetFullPath(libraryPath);
            if (!File.Exists(libraryPath))
            {
                throw new Exception(string.Format("Could not find file {0}", libraryPath));
            }
            var libPtr = LoadLibrary(libraryPath);
            if (libPtr == IntPtr.Zero)
            {
                throw new Exception(string.Format("Failed to load native library {0}", libraryPath));
            }
            return libPtr;
        }
        static void UnloadTypeTreeLib(IntPtr libPtr)
        {
            if (!FreeLibrary(libPtr))
            {
                Debug.LogError("Could not unload native library");
            }
        }
        public static TRet Invoke<TRet, TDel>(params object[] pars)
        {
            var libPtr = LoadTypeTreeLib();
            IntPtr funcPtr = GetProcAddress(libPtr, typeof(TDel).Name);
            if (funcPtr == IntPtr.Zero)
            {
                Debug.LogWarning("Could not gain reference to method address.");
                UnloadTypeTreeLib(libPtr);
                return default(TRet);
            }
            var func = Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(TDel));
            var result = (TRet)func.DynamicInvoke(pars);
            UnloadTypeTreeLib(libPtr);
            return result;
        }

        public static void Invoke<T>(params object[] pars)
        {
            var libPtr = LoadTypeTreeLib();
            IntPtr funcPtr = GetProcAddress(libPtr, typeof(T).Name);
            if (funcPtr == IntPtr.Zero)
            {
                Debug.LogWarning("Could not gain reference to method address.");
                UnloadTypeTreeLib(libPtr);
                return;
            }
            var func = Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(T));
            func.DynamicInvoke(pars);
            UnloadTypeTreeLib(libPtr);
        }

        [DllImport("kernel32", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport("kernel32")]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);
    }
}
