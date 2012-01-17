﻿// Copyright (c) 2011,cd-team.org.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace SiriProxy.Plugins.Interface4Net
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct PlistPackageArgs
    {
        /// char[37]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 37)]
        public byte[] aceId;

        /// char[37]
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 37)]
        public byte[] refId;

        /// char*
        public IntPtr sendText;

        /// char*
        public IntPtr responseText;

        public static explicit operator CommandCallArgs(PlistPackageArgs obj)
        {
            if (obj.aceId==null || obj.aceId.Length !=37)
            {
                throw new Exception("aceId的长度应该等于37"); 
            }
            CommandCallArgs ret = new CommandCallArgs();
            List<byte> cache = new List<byte>();
            byte c = 0;
            for (int i = 0; i < obj.aceId.Length; i++)
            {
                c = obj.aceId[i];
                if (c != (byte)'\0')
                {
                    cache.Add(c);
                }
                else
                {
                    break;
                }
            }
            ret.AceId = Encoding.Default.GetString(cache.ToArray());

            if (obj.refId == null || obj.refId.Length != 37)
            {
                throw new Exception("aceId的长度应该等于37");
            }
            cache.Clear();
            for (int i = 0; i < obj.refId.Length; i++)
            {
                c = obj.refId[i];
                if (c != (byte)'\0')
                {
                    cache.Add(c);
                }
                else
                {
                    break;
                }
            }
            ret.RefId = Encoding.Default.GetString(cache.ToArray());

            if (obj.sendText != IntPtr.Zero)
            {
                ret.SendText = StringHelper.PtrToStringAnsi(obj.sendText);
            }
            if (obj.responseText != IntPtr.Zero)
            {
                ret.ResponseText = StringHelper.PtrToStringAnsi(obj.responseText);
            }
            return ret;
        }
    }

    public class CommandCallArgs : EventArgs
    {
        /// <summary>
        /// 命令匹配时的AceId字段
        /// </summary>
        public string AceId { get; set; }

        /// <summary>
        /// 命令匹配时的RefId字段
        /// </summary>
        public string RefId { get; set; }

        /// <summary>
        /// 命令文本
        /// </summary>
        public string SendText { get; set; }

        /// <summary>
        /// 服务器的原始响应文本
        /// </summary>
        public string ResponseText { get; set; }

        /// <summary>
        /// 触发事件后对此字段进行赋值，以改变原有的plist数据
        /// </summary>
        public string ResultText { get; set; }

        //返回的文本类型：text还是plist
        public ResultType Type { get; set; }

        public static explicit operator PlistPackageArgs(CommandCallArgs obj) 
        {
            PlistPackageArgs ret = new PlistPackageArgs();
            if (string.IsNullOrEmpty(obj.AceId))
            {
                throw new Exception("AceId的长度应该大于0且小于36");
            }
            byte[] cache = Encoding.Default.GetBytes(obj.AceId);
            if (cache.Length > 36)
            {
                throw new Exception("AceId的长度应该大于0且小于36");
            }
            ret.aceId=new byte[37];
            Buffer.BlockCopy(cache, 0, ret.aceId, 0, cache.Length);

            if (string.IsNullOrEmpty(obj.RefId))
            {
                throw new Exception("RefId的长度应该大于0且小于36");
            }
            cache = Encoding.Default.GetBytes(obj.RefId);
            if (cache.Length > 36)
            {
                throw new Exception("RefId的长度应该大于0且小于36");
            }
            ret.refId = new byte[37];
            Buffer.BlockCopy(cache, 0, ret.refId, 0, cache.Length);

            if (!string.IsNullOrEmpty(obj.SendText))
            {
                ret.sendText = Marshal.StringToHGlobalAnsi(obj.SendText);
            }
            if (!string.IsNullOrEmpty(obj.ResponseText))
            {
                ret.responseText = Marshal.StringToHGlobalAnsi(obj.ResponseText);
            }
            return ret;
        }
    }
}