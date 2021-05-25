#ifndef VIEW_ABSTRACT_ATLAS_H_
#define VIEW_ABSTRACT_ATLAS_H_

#include <QImage>
#include <QOpenGLTexture>
#include <QPixmap>

class AbstractAtlas : public QOpenGLTexture {
 public:
  explicit AbstractAtlas(QOpenGLTexture::Target target)
      : QOpenGLTexture(target) {}

  virtual void Init() = 0;

 protected:
  static QImage GetImage(const QPixmap& pixmap) {
    QImage image = pixmap.toImage();
    if (image.format() != QImage::Format_RGBA8888) {
      return image.convertToFormat(QImage::Format_RGBA8888);
    }
    return image;
  }
};

#endif  // VIEW_ABSTRACT_ATLAS_H_
