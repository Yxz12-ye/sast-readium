#include "QGraphicsPDFViewerToolbar.h"
#include <QGroupBox>
#include "QGraphicsPDFViewer.h"

QGraphicsPDFViewerToolbar::QGraphicsPDFViewerToolbar(QWidget* parent)
    : QWidget(parent), m_viewer(nullptr), m_updatingControls(false) {
    setupUI();
    setupConnections();
    updateControls(false, 0, 0, 1.0, 0);
}

void QGraphicsPDFViewerToolbar::setViewer(QGraphicsPDFViewer* viewer) {
    m_viewer = viewer;

    if (m_viewer) {
        // Connect viewer signals to toolbar updates
        connect(m_viewer, &QGraphicsPDFViewer::documentChanged,
                [this](bool hasDocument) {
                    if (hasDocument) {
                        updateControls(true, m_viewer->getCurrentPage(),
                                       m_viewer->getPageCount(),
                                       m_viewer->getZoomFactor(),
                                       m_viewer->getRotation());
                    } else {
                        updateControls(false, 0, 0, 1.0, 0);
                    }
                });

        connect(m_viewer, &QGraphicsPDFViewer::currentPageChanged,
                [this](int pageNumber) {
                    updateControls(m_viewer->hasDocument(), pageNumber,
                                   m_viewer->getPageCount(),
                                   m_viewer->getZoomFactor(),
                                   m_viewer->getRotation());
                });

        connect(m_viewer, &QGraphicsPDFViewer::zoomChanged,
                [this](double zoomFactor) {
                    updateControls(m_viewer->hasDocument(),
                                   m_viewer->getCurrentPage(),
                                   m_viewer->getPageCount(), zoomFactor,
                                   m_viewer->getRotation());
                });

        connect(m_viewer, &QGraphicsPDFViewer::rotationChanged,
                [this](int rotation) {
                    updateControls(m_viewer->hasDocument(),
                                   m_viewer->getCurrentPage(),
                                   m_viewer->getPageCount(),
                                   m_viewer->getZoomFactor(), rotation);
                });
    }
}

void QGraphicsPDFViewerToolbar::updateControls(bool hasDocument,
                                               int currentPage, int pageCount,
                                               double zoomFactor,
                                               int rotation) {
    m_updatingControls = true;

    // Enable/disable all controls based on document presence
    bool enabled = hasDocument;

    // Page navigation controls
    m_firstPageBtn->setEnabled(enabled && currentPage > 0);
    m_prevPageBtn->setEnabled(enabled && currentPage > 0);
    m_nextPageBtn->setEnabled(enabled && currentPage < pageCount - 1);
    m_lastPageBtn->setEnabled(enabled && currentPage < pageCount - 1);
    m_pageNumberSpinBox->setEnabled(enabled);
    m_pageCountLabel->setText(QString("/ %1").arg(pageCount));
    if (enabled) {
        m_pageNumberSpinBox->setRange(1, pageCount);
        m_pageNumberSpinBox->setValue(currentPage + 1);
    } else {
        m_pageNumberSpinBox->setRange(1, 1);
        m_pageNumberSpinBox->setValue(1);
    }

    // Zoom controls
    m_zoomInBtn->setEnabled(enabled);
    m_zoomOutBtn->setEnabled(enabled);
    m_zoomSlider->setEnabled(enabled);
    m_zoomPercentageSpinBox->setEnabled(enabled);
    m_fitWidthBtn->setEnabled(enabled);
    m_fitHeightBtn->setEnabled(enabled);
    m_fitPageBtn->setEnabled(enabled);

    int zoomPercentage = qRound(zoomFactor * 100);
    m_zoomSlider->setValue(zoomPercentage);
    m_zoomPercentageSpinBox->setValue(zoomPercentage);

    // Rotation controls
    m_rotateLeftBtn->setEnabled(enabled);
    m_rotateRightBtn->setEnabled(enabled);

    // View mode control
    m_viewModeComboBox->setEnabled(enabled);

    m_updatingControls = false;
}

void QGraphicsPDFViewerToolbar::setViewMode(int mode) {
    m_viewModeComboBox->setCurrentIndex(mode);
}

void QGraphicsPDFViewerToolbar::onPageNumberSpinBoxChanged(int value) {
    if (m_updatingControls)
        return;

    emit pageNumberChanged(value - 1);
}

void QGraphicsPDFViewerToolbar::onZoomSliderChanged(int value) {
    if (m_updatingControls)
        return;

    emit zoomPercentageChanged(value);
}

void QGraphicsPDFViewerToolbar::onZoomPercentageSpinBoxChanged(int value) {
    if (m_updatingControls)
        return;

    emit zoomPercentageChanged(value);
}

void QGraphicsPDFViewerToolbar::onViewModeComboBoxChanged(int index) {
    if (m_updatingControls)
        return;

    emit viewModeChanged(index);
}

void QGraphicsPDFViewerToolbar::setupUI() {
    setObjectName("pdfViewerToolbar");

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(8, 8, 8, 8);
    m_mainLayout->setSpacing(8);

    // Page navigation controls
    QGroupBox* navGroup = new QGroupBox("页面导航", this);
    QHBoxLayout* navLayout = new QHBoxLayout(navGroup);

    m_firstPageBtn = new QPushButton("⏮", navGroup);
    m_prevPageBtn = new QPushButton("◀", navGroup);
    m_pageNumberSpinBox = new QSpinBox(navGroup);
    m_pageCountLabel = new QLabel("/ 0", navGroup);
    m_nextPageBtn = new QPushButton("▶", navGroup);
    m_lastPageBtn = new QPushButton("⏭", navGroup);

    m_firstPageBtn->setFixedSize(32, 32);
    m_prevPageBtn->setFixedSize(32, 32);
    m_nextPageBtn->setFixedSize(32, 32);
    m_lastPageBtn->setFixedSize(32, 32);
    m_pageNumberSpinBox->setMaximumWidth(60);

    m_firstPageBtn->setToolTip("第一页");
    m_prevPageBtn->setToolTip("上一页");
    m_nextPageBtn->setToolTip("下一页");
    m_lastPageBtn->setToolTip("最后一页");

    navLayout->addWidget(m_firstPageBtn);
    navLayout->addWidget(m_prevPageBtn);
    navLayout->addWidget(m_pageNumberSpinBox);
    navLayout->addWidget(m_pageCountLabel);
    navLayout->addWidget(m_nextPageBtn);
    navLayout->addWidget(m_lastPageBtn);

    // Zoom controls
    QGroupBox* zoomGroup = new QGroupBox("缩放", this);
    QHBoxLayout* zoomLayout = new QHBoxLayout(zoomGroup);

    m_zoomOutBtn = new QPushButton("🔍-", zoomGroup);
    m_zoomInBtn = new QPushButton("🔍+", zoomGroup);
    m_zoomSlider = new QSlider(Qt::Horizontal, zoomGroup);
    m_zoomPercentageSpinBox = new QSpinBox(zoomGroup);
    m_fitWidthBtn = new QPushButton("📏", zoomGroup);
    m_fitHeightBtn = new QPushButton("📐", zoomGroup);
    m_fitPageBtn = new QPushButton("🗎", zoomGroup);

    m_zoomOutBtn->setFixedSize(32, 32);
    m_zoomInBtn->setFixedSize(32, 32);
    m_fitWidthBtn->setFixedSize(32, 32);
    m_fitHeightBtn->setFixedSize(32, 32);
    m_fitPageBtn->setFixedSize(32, 32);

    m_zoomSlider->setRange(10, 500);  // 10% to 500%
    m_zoomSlider->setValue(100);
    m_zoomSlider->setMinimumWidth(120);

    m_zoomPercentageSpinBox->setRange(10, 500);
    m_zoomPercentageSpinBox->setValue(100);
    m_zoomPercentageSpinBox->setSuffix("%");
    m_zoomPercentageSpinBox->setMinimumWidth(80);
    m_zoomPercentageSpinBox->setMaximumWidth(80);

    m_zoomOutBtn->setToolTip("缩小");
    m_zoomInBtn->setToolTip("放大");
    m_fitWidthBtn->setToolTip("适合宽度");
    m_fitHeightBtn->setToolTip("适合高度");
    m_fitPageBtn->setToolTip("适合页面");

    zoomLayout->addWidget(m_zoomOutBtn);
    zoomLayout->addWidget(m_zoomInBtn);
    zoomLayout->addWidget(m_zoomSlider);
    zoomLayout->addWidget(m_zoomPercentageSpinBox);
    zoomLayout->addWidget(m_fitWidthBtn);
    zoomLayout->addWidget(m_fitHeightBtn);
    zoomLayout->addWidget(m_fitPageBtn);

    // Rotation controls
    QGroupBox* rotateGroup = new QGroupBox("旋转", this);
    QHBoxLayout* rotateLayout = new QHBoxLayout(rotateGroup);

    m_rotateLeftBtn = new QPushButton("↺", rotateGroup);
    m_rotateRightBtn = new QPushButton("↻", rotateGroup);

    m_rotateLeftBtn->setFixedSize(32, 32);
    m_rotateRightBtn->setFixedSize(32, 32);

    m_rotateLeftBtn->setToolTip("向左旋转90度");
    m_rotateRightBtn->setToolTip("向右旋转90度");

    rotateLayout->addWidget(m_rotateLeftBtn);
    rotateLayout->addWidget(m_rotateRightBtn);

    // View mode control
    QGroupBox* viewModeGroup = new QGroupBox("查看模式", this);
    QHBoxLayout* viewModeLayout = new QHBoxLayout(viewModeGroup);

    m_viewModeComboBox = new QComboBox(viewModeGroup);
    m_viewModeComboBox->addItem("单页视图");
    m_viewModeComboBox->addItem("连续滚动");
    m_viewModeComboBox->addItem("双页视图");
    m_viewModeComboBox->addItem("连续双页");

    viewModeLayout->addWidget(m_viewModeComboBox);

    // Add all groups to main layout
    m_mainLayout->addWidget(navGroup);
    m_mainLayout->addWidget(zoomGroup);
    m_mainLayout->addWidget(rotateGroup);
    m_mainLayout->addWidget(viewModeGroup);

    // Add stretch to push controls to left
    m_mainLayout->addStretch(1);
}

void QGraphicsPDFViewerToolbar::setupConnections() {
    // Page navigation connections
    connect(m_firstPageBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::firstPageRequested);
    connect(m_prevPageBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::previousPageRequested);
    connect(m_nextPageBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::nextPageRequested);
    connect(m_lastPageBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::lastPageRequested);
    connect(m_pageNumberSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &QGraphicsPDFViewerToolbar::onPageNumberSpinBoxChanged);

    // Zoom connections
    connect(m_zoomInBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::zoomInRequested);
    connect(m_zoomOutBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::zoomOutRequested);
    connect(m_zoomSlider, &QSlider::valueChanged, this,
            &QGraphicsPDFViewerToolbar::onZoomSliderChanged);
    connect(m_zoomPercentageSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged), this,
            &QGraphicsPDFViewerToolbar::onZoomPercentageSpinBoxChanged);
    connect(m_fitWidthBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::zoomToWidthRequested);
    connect(m_fitHeightBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::zoomToHeightRequested);
    connect(m_fitPageBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::zoomToFitRequested);

    // Rotation connections
    connect(m_rotateLeftBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::rotateLeftRequested);
    connect(m_rotateRightBtn, &QPushButton::clicked, this,
            &QGraphicsPDFViewerToolbar::rotateRightRequested);

    // View mode connections
    connect(m_viewModeComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &QGraphicsPDFViewerToolbar::onViewModeComboBoxChanged);
}