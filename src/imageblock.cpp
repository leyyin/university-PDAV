#include "imageblock.hpp"

Imageblock::Imageblock(quint32 frame_id, QPoint position, QImage image)
    : m_frame_id(frame_id), m_position(position), m_image(image)
{
}

QImage Imageblock::getImage() const { return m_image; }

bool Imageblock::operator==(const Imageblock& other)
{
    qreal mse        = 0.0;
    auto other_image = other.getImage();

    for (int x = 0; x < m_image.width(); x++)
    {
        for (int y = 0; y < m_image.height(); y++)
        {
            mse += pow(util::yComponent(m_image.pixel(x, y)) - util::yComponent(other_image.pixel(x, y)), 2.0);
        }
    }
    mse /= (m_image.width() * m_image.height());

    return mse < constants::DEFAULT_IMAGE_DIFF_THRESHOLD;
}

bool Imageblock::operator!=(const Imageblock& other) { return !(*this == other); }
