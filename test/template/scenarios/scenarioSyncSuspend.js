var CodePushWrapper = require("../codePushWrapper.js");
import CodePush from "@itspar/codepush-sdk";

module.exports = {
    startTest: function (testApp) {
        CodePushWrapper.sync(testApp, undefined, undefined,
            { installMode: CodePush.InstallMode.ON_NEXT_SUSPEND });
    },

    getScenarioName: function () {
        return "Sync Suspend";
    }
};
