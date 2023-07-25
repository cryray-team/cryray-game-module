#pragma once
// 1: default
// 1.5: check next chunk first heuristics
// 2: vector population heuristics
// 3: dynamic map population heuristics

struct IReaderBase_Test {};
#pragma warning (disable:4701)

template <typename T>
IC u32 IReaderBase<T>::find_chunk (u32 ID, BOOL* bCompressed)
{
    u32 dwSize, dwType;

    bool success = false;

    if (m_last_pos != 0)
    {
        impl().seek(m_last_pos);
        dwType = r_u32();
        dwSize = r_u32();

        if ((dwType & (~CFS_CompressMark)) == ID)
        {
            success = true;
        }
    }

    if (!success)
    {
        constexpr int size = sizeof(u32) * 2;

        rewind();

        while (impl().elapsed() >= size)
        {
            dwType = r_u32();
            dwSize = r_u32();
            if ((dwType & (~CFS_CompressMark)) == ID)
            {
                success = true;
                break;
            }
            else
            {
                if ((ID & 0x7ffffff0) == 0x810) // is it a thm chunk ID?
                {
                    const u32 pos = (u32)impl().tell();
                    const u32 size = (u32)impl().length();
                    u32 length = dwSize;

                    if (pos + length != size) // not the last chunk in the file?
                    {
                        bool ok = true;
                        if (pos + length > size - 8) ok = false; // size too large?
                        if (ok)
                        {
                            impl().seek(pos + length);
                            if ((r_u32() & 0x7ffffff0) != 0x810) ok = false; // size too small?
                        }
                        if (!ok) // size incorrect?
                        {
                            length = 0;
                            while (pos + length < size) // find correct size, up to eof
                            {
                                impl().seek(pos + length);
                                if (pos + length <= size - 8 && (r_u32() & 0x7ffffff0) == 0x810) break; // found start of next section
                                length++;
                            }
                            Msg("! THM chunk %d fixed, wrong size = %d, correct size = %d", ID, dwSize, length);
                        }
                    }

                    impl().seek(pos); // go back to beginning of chunk
                    dwSize = length; // use correct(ed) size
                }
                impl().advance(dwSize);
            }
        }

        if (!success)
        {
            m_last_pos = 0;
            return 0;
        }
    }

#ifdef DEBUG
    VERIFY((u32)impl().tell() + dwSize <= (u32)impl().length());
#endif

    if (bCompressed) *bCompressed = dwType & CFS_CompressMark;

    const u32 dwPos =(u32) impl().tell();
    if (dwPos + dwSize < (u32)impl().length())
    {
        m_last_pos = dwPos + dwSize;
    }
    else
    {
        m_last_pos = 0;
    }

    return dwSize;
}

#pragma warning (default:4701)
