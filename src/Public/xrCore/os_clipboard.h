#pragma once

namespace os_clipboard
{
XRCORE_API void copy_to_clipboard(LPCSTR buf);
XRCORE_API void paste_from_clipboard(LPSTR buf, u32 const& buf_size);
XRCORE_API void update_clipboard(LPCSTR str);
} // namespace os_clipboard
