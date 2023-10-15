#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

namespace bmpr
{
#pragma pack(push, 1)
    struct Header
    {
        std::uint16_t signature;
        std::uint32_t file_size;
        std::uint32_t reserved;
        std::uint32_t data_offset;
        std::uint32_t info_header_size;
        std::int32_t width;
        std::int32_t height;
        std::uint16_t planes;
        std::uint16_t bit_depth;
        std::uint32_t compression;
        std::uint32_t img_size;
        std::int32_t xpix_per_m;
        std::int32_t ypix_per_m;
        std::uint32_t colors_used;
        std::uint32_t colors_important;
    };
#pragma pack(pop)

    struct Color
    {
        uint8_t r, g, b;
        Color() : r(0), g(0), b(0) {}
        Color(uint8_t rgb) : r(rgb), g(rgb), b(rgb) {}
        Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    };

    class Image
    {
    public:
        // Initialize an image with a width and height in pixels
        Image(std::size_t width, std::size_t height);
        // Set the color of a specific pixel
        void Set(std::int32_t x, std::int32_t y, const Color &color);
        // Set the color of a specific pixel only if it's valid
        void SetSafe(std::int32_t x, std::int32_t y, const Color &color);
        // Sets the whole image to the specific color
        void Clear(const Color &color);
        // Draws a line from x1;y1 to x2;y2
        void DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, Color color);
        // Draws a line from x1;y1 to x2;y2 with a certain thickness
        void DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, int thickness, Color color);
        // Returns image width in pixels
        std::int32_t Width() const noexcept;
        // Returns image height in pixels
        std::int32_t Height() const noexcept;
        // Saves the image to file. NOTE: Include the .bmp extension
        bool Save(const std::string &path);
        // Rotates the image 180 degrees
        void Rotate180();
        // Flips the image horizontally
        void FlipHorizontally();
        // Flips the image vertically
        void FlipVertically();

    private:
        std::vector<Color> m_data;
        std::int32_t m_width = 0, m_height = 0;
    };
}

// Implementations
namespace bmpr
{
    Image::Image(std::size_t width, std::size_t height) : m_data(width * height), m_width{static_cast<std::int32_t>(width)}, m_height{static_cast<std::int32_t>(height)} {}

    void Image::Set(std::int32_t x, std::int32_t y, const Color &color)
    {
        m_data[static_cast<std::size_t>(y) * m_width + x] = color;
    }

    void Image::SetSafe(std::int32_t x, std::int32_t y, const Color &color)
    {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height)
        {
            std::size_t index = static_cast<std::size_t>(y) * m_width + x;
            m_data[index] = color;
        }
    }

    void Image::DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, Color color)
    {
        int delta_x = abs(x2 - x1);
        int delta_y = abs(y2 - y1);
        int sign_x = (x1 < x2) ? 1 : -1;
        int sign_y = (y1 < y2) ? 1 : -1;

        int error = delta_x - delta_y;

        int x = x1;
        int y = y1;

        while (x != x2 || y != y2)
        {
            // Set the pixel color
            SetSafe(x, y, color);

            int error2 = error * 2;

            if (error2 > -delta_y)
            {
                error -= delta_y;
                x += sign_x;
            }

            if (error2 < delta_x)
            {
                error += delta_x;
                y += sign_y;
            }
        }
    }

    void Image::DrawLine(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, int thickness, Color color)
    {
        // Check if the line thickness is less than 1
        if (thickness < 1)
            thickness = 1;

        int delta_x = abs(x2 - x1);
        int delta_y = abs(y2 - y1);
        int sign_x = (x1 < x2) ? 1 : -1;
        int sign_y = (y1 < y2) ? 1 : -1;

        int error = delta_x - delta_y;

        int x = x1;
        int y = y1;

        while (x != x2 || y != y2)
        {
            // Set the pixel color for the current thickness
            for (int i = 0; i < thickness; ++i)
            {
                for (int j = 0; j < thickness; ++j)
                {
                    SetSafe(x + i, y + j, color);
                }
            }

            int error2 = error * 2;

            if (error2 > -delta_y)
            {
                error -= delta_y;
                x += sign_x;
            }

            if (error2 < delta_x)
            {
                error += delta_x;
                y += sign_y;
            }
        }
    }

    void Image::Clear(const Color &color)
    {
        for (std::size_t i = 0; i < m_width; i++)
            for (std::size_t j = 0; j < m_height; j++)
                Set(i, j, color);
    }

    std::int32_t Image::Width() const noexcept { return m_width; }

    std::int32_t Image::Height() const noexcept { return m_height; }

    bool Image::Save(const std::string &path)
    {
        const std::int32_t row_size = m_width * 3 + m_width % 4;
        const std::uint32_t bmp_size = row_size * m_height;

        const Header header = {0x4d42,                                                // Signature ('BM')
                               static_cast<std::uint32_t>(bmp_size + sizeof(Header)), // File size in bytes
                               0,                                                     // reserved (unused)
                               sizeof(Header),                                        // Offset from beginning of file to the beginning of the bitmap data
                               40,                                                    // Size of InfoHeader
                               m_width,                                               // Horizontal width of bitmap in pixels
                               m_height,                                              // Vertical height of bitmap in pixels
                               1,                                                     // Number of Planes
                               24,                                                    // Bit-depth
                               0,                                                     // Compression (0 = none)
                               bmp_size,                                              // Size of Image
                               0,                                                     // Horizontal resolution: Pixels/meter
                               0,                                                     // Vertical resolution: Pixels/meter
                               0,                                                     // Colors used
                               0};                                                    // Number of important colors (0 = all)

        if (std::ofstream ofs{path, std::ios_base::binary})
        {
            ofs.write(reinterpret_cast<const char *>(&header), sizeof(header));

            std::vector<std::uint8_t> line(row_size);

            for (std::int32_t y = m_height - 1; -1 < y; --y)
            {
                size_t pos = 0;

                for (std::int32_t x = 0; x < m_width; ++x)
                {
                    const Color &col = m_data[static_cast<std::size_t>(y) * m_width + x];
                    line[pos++] = col.b;
                    line[pos++] = col.g;
                    line[pos++] = col.r;
                }

                ofs.write(reinterpret_cast<const char *>(line.data()), line.size());
            }

            return true;
        }
        else
            return false;
    }

    void Image::Rotate180()
    {
        std::reverse(m_data.begin(), m_data.end());
    }

    void Image::FlipHorizontally()
    {
        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_width / 2; ++x)
            {
                int index_1D_left = y * m_width + x;
                int index_1D_right = y * m_width + (m_width - 1 - x);
                std::swap(m_data[index_1D_left], m_data[index_1D_right]);
            }
        }
    }

    void Image::FlipVertically()
    {
        for (int y = 0; y < m_height / 2; ++y)
        {
            for (int x = 0; x < m_width; ++x)
            {
                int index_1D_top = y * m_width + x;
                int index_1D_bottom = (m_height - 1 - y) * m_width + x;
                std::swap(m_data[index_1D_top], m_data[index_1D_bottom]);
            }
        }
    }
}