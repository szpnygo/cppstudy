#include "app.h"
#include <gtest/gtest.h>

using namespace easywebrtc;

class AppTest : public ::testing::Test {
protected:
  easywebrtc::WebRTCApp *app;
};

TEST_F(AppTest, ParseConfiguration) { app->parseConfiguration(); }