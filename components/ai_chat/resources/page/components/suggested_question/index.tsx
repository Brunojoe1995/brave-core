// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import Button from "@brave/leo/react/button";
import { useConversation } from "../../state/conversation_context";
import styles from './style.module.scss'

export default function SuggestedQuestion({ question }: { question: string }) {
    const context = useConversation()
    const handleQuestionSubmit = () => {
        context.conversationHandler?.submitHumanConversationEntry(question)
    }

    // return <div className={styles.suggestion} onClick={handleQuestionSubmit}>
    //     <Icon className={styles.icon}>💰</Icon>
    //     {question}
    // </div>
    return <Button
        kind='outline'
        size='small'
        onClick={() => handleQuestionSubmit()}
        isDisabled={context.shouldDisableUserInput}
        className={styles.suggestion}>
        <div className={styles.container}>
            <div className={styles.icon}>
                💰
            </div>
            <span className={styles.buttonText}>{question}</span>
        </div>
    </Button>
}
