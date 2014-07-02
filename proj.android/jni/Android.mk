LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BaseScene.cpp \
                   ../../Classes/EventsAPIScene.cpp \
                   ../../Classes/FBActionScene.cpp \
                   ../../Classes/FeedAPIScene.cpp \
                   ../../Classes/FriendListScene.cpp \
                   ../../Classes/FriendsScene.cpp \
                   ../../Classes/HelpScene.cpp \
                   ../../Classes/HomeScene.cpp \
                   ../../Classes/OpenGraphAPIScene.cpp \
                   ../../Classes/ProfileScene.cpp \
                   ../../Classes/RequestAPIScene.cpp \
                   ../../Classes/RequestListScene.cpp \
                   ../../Classes/ScoreAPIScene.cpp \
                   ../../Classes/ShareDialogScene.cpp \
                   ../../Classes/SplashScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../ezibyte/ezisocial/include \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += ezibyte_social_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,network)
$(call import-module,extensions)
$(call import-module,../../ezibyte/ezisocial/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)
