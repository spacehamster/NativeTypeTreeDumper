using System;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEditor.Compilation;
using UnityEngine;

namespace TypeTreeTools
{
    static class NativeCompiler
    {
        const string ProjectDirectory = "NativeTypeTreeTools";

        const string ProjectName = "TypeTreeTools";
        static readonly string[] VersionSplitChars =
{
            ".", "a", "b", "rc", "f"
        };
        public static bool Is64BitProcess { get { return IntPtr.Size == 8; } }

        public static string GetFullPath(string fileName)
        {
            if (File.Exists(fileName))
                return Path.GetFullPath(fileName);

            var values = Environment.GetEnvironmentVariable("PATH");
            foreach (var path in values.Split(Path.PathSeparator))
            {
                var fullPath = Path.Combine(path, fileName);
                if (File.Exists(fullPath))
                    return fullPath;
            }
            return null;
        }
        static void Start(string exe, string arguments)
        {
            var process = new System.Diagnostics.Process
            {
                StartInfo = new System.Diagnostics.ProcessStartInfo
                {
                    FileName = exe,
                    Arguments = arguments,
                    WorkingDirectory = ProjectDirectory,
                    UseShellExecute = false,
                    RedirectStandardOutput = true
                }
            };

            process.Start();
            string strOutput = process.StandardOutput.ReadToEnd();
            process.WaitForExit();
            if(process.ExitCode != 0)
            {
                Debug.LogErrorFormat("Error running command {0} {1}",
                    exe, arguments);
            }
            if (!Directory.Exists("Logs")) Directory.CreateDirectory("Logs");
            File.WriteAllText("Logs/BuildResult.txt", strOutput);
        }
        static void WriteDefines()
        {
            using (var sw = new StreamWriter("NativeTypeTreeTools/NativeTypeTreeTools/Defines.h"))
            {
                var version = Application.unityVersion;
                var split = Application.unityVersion.Split(VersionSplitChars, StringSplitOptions.RemoveEmptyEntries);
                sw.WriteLine("#pragma once");
                sw.WriteLine("#define EditorVersion {0}", version);
                sw.WriteLine("#define EditorVersionMajor {0}", split[0]);
                sw.WriteLine("#define EditorVersionMinor {0}", split[1]);
                sw.WriteLine("#define EditorContentsDir {0}", EditorApplication.applicationContentsPath);
                var assembly = CompilationPipeline.GetAssemblies().First(a => a.name == "Assembly-CSharp-Editor");
                foreach (var define in assembly.defines)
                {
                    sw.WriteLine("#define {0}", define);
                }
            }
        }
        [MenuItem("Tools/Build NativeTypeTreeTools.dll")]
        static void BuildNativeTypeTreeTools()
        {
            WriteDefines();
            var msbuild = GetFullPath("msbuild.exe");
            msbuild = @"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe";
            if (!File.Exists(msbuild))
            {
                throw new Exception("Could not find executable msbuild.exe in path");
            }
            var outDir = Is64BitProcess ?
                Path.GetFullPath(@"CustomPlugins\x86_64") :
                Path.GetFullPath(@"CustomPlugins\x86");
            var platform = Is64BitProcess ? "x64" : "x86";
            var arguments = string.Format("NativeTypeTreeTools.sln /t:Build /p:Configuration=Debug /p:OutDir={0} /p:Platform={1}",
                outDir, platform);
            Start(msbuild, arguments);
        }
    }
}
